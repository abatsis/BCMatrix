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
#include <limits>

typedef Eigen::Triplet<double> T;
using matrixOperation = Spectra::SparseGenMatProd<double>;

static auto exportGraph(LatticeGraph &graph, std::string filePath)
{
    if (filePath.empty())
        return;

    std::ofstream newFile(filePath);

    auto edges = graph.getEdges();
    for (auto &edge : *edges)
    {
        newFile << edge.start.position << "," << edge.end.position << "," << edge.weight << "\n";
    }

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
    int imin = std::numeric_limits<int>::min();
    int imax = std::numeric_limits<int>::max();
    std::cout << "(min, max)= ("
              << imin << ", " << imax << ")" << std::endl;
    std::string filePath = argv[1];
    std::string exportPath = argv[2];
    DataBase database;
    database.readFile(filePath);
    auto randomWalk = database.getRandomWalk();
    auto boundary = database.getBoundary();
    auto shortingProjection = database.getShortingProjection();

    auto graph = LatticeGraph(randomWalk, boundary, shortingProjection);
    auto numberOfNodes = graph.numberOfNodes();
    exportGraph(graph, exportPath);

    auto matrix = graphToMatrix(graph);
    std::cout << "Matrix complete. Size is " << numberOfNodes << std::endl;
    graph.clear();

    auto convergenceSpeed = std::min(20, numberOfNodes - 2);
    auto spectralRadius = computeSpectralRadius(matrix, convergenceSpeed);
    std::cout << "Spectral radius: " << spectralRadius << std::endl;
}
