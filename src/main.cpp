#include "transformation.h"
#include "randomWalk.h"
#include <iostream>
#include "projection.h"
#include "boundary.h"
#include "latticeGraph.h"
#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>
#include "DataBase.h"

typedef Eigen::Triplet<double> T;
using matrixOperation = Spectra::SparseGenMatProd<double>;

static auto exportGraph(LatticeGraph &graph, std::string exportDir)
{
    if (exportDir.empty())
        return;

    auto filePath = exportDir + "/graph.csv";
    std::ofstream newFile(filePath);

    auto edges = graph.getEdges();
    for (auto &edge : *edges)
    {
        newFile << edge.start.position << "," << edge.end.position << "," << edge.weight << "\n";
    }

    newFile.close();
}

static auto exportReport(double const &spectralRadius, std::string exportDir)
{
    auto reportPath = exportDir + "/report.txt";
    std::ofstream newFile(reportPath);
    newFile << "Spectral radius: " << spectralRadius << std::endl;
    newFile.close();
}

static auto graphToMatrix(LatticeGraph &graph) -> Eigen::SparseMatrix<double>
{
    auto edges = graph.getEdges();
    auto numberOfEdges = edges->size();
    auto numberOfNodes = graph.numberOfNodes();

    std::vector<T> tripletList;
    tripletList.reserve(numberOfEdges);
    for (auto &edge : *edges)
    {
        auto entryRow = edge.start.position;
        auto entryColumn = edge.end.position;
        auto entry = edge.weight;
        tripletList.push_back(T(entryRow, entryColumn, entry));
    }
    Eigen::SparseMatrix<double> matrix(numberOfNodes, numberOfNodes);
    matrix.setFromTriplets(tripletList.begin(), tripletList.end());
    return matrix;
}

static auto computeSpectralRadius(Eigen::SparseMatrix<double> &matrix, int const &convergenceSpeed) -> double
{
    matrixOperation op(matrix);
    Spectra::GenEigsSolver<matrixOperation> eigs(op, 1, convergenceSpeed);
    eigs.init();
    eigs.compute(Spectra::SortRule::LargestMagn);
    Eigen::VectorXcd eigenvalues;
    eigenvalues = eigs.eigenvalues();
    auto spectralRadius = eigenvalues.cwiseAbs().maxCoeff();
    return spectralRadius;
}

int main(int argc, char *argv[])
{
    std::string filePath = argv[1];
    std::string exportDir = argv[2];

    DataBase database;
    database.readFile(filePath);
    auto randomWalk = database.getRandomWalk();
    auto boundary = database.getBoundary();
    auto shortingProjection = database.getShortingProjection();

    auto graph = LatticeGraph(randomWalk, boundary, shortingProjection);
    auto numberOfNodes = graph.numberOfNodes();
    exportGraph(graph, exportDir);

    auto matrix = graphToMatrix(graph);
    std::cout << "Matrix complete. Size is " << numberOfNodes << std::endl;
    graph.clear();

    auto convergenceSpeed = std::min(20, numberOfNodes - 2);
    auto spectralRadius = computeSpectralRadius(matrix, convergenceSpeed);
    exportReport(spectralRadius, exportDir);
    std::cout << "*********************" << std::endl
              << "Finished successfully" << std::endl
              << "*********************" << std::endl;
}
