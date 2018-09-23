#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include <iterator>

#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkMath.h>
#include <vtkDoubleArray.h>
#include <vtkStringArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkFieldData.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include "csvRead.cpp"

int main(int, char *argv[])
{
    char *inputFilename = strdup(argv[1]);
    char *outputFilename = strdup(argv[2]);
    int debug = std::stoi(strdup(argv[3]));
    char *scalarName = "Z-dimension";
    std::map<double, double> m;

    // Read file
    vtkSmartPointer<vtkPolyDataReader> polyIn = vtkSmartPointer<vtkPolyDataReader>::New();
    polyIn->SetFileName(inputFilename); 
    polyIn->Update();
    vtkSmartPointer<vtkPolyData> polydata = polyIn->GetOutput();
    unsigned int nPoints = polydata->GetNumberOfPoints();
    if (debug) {
        printf("Number of Points %d\n", nPoints);
    }

    unsigned int numberOfArrays = polydata->GetPointData()->GetNumberOfArrays();
    if (debug) {
        printf("Number of Arrays %d\n", numberOfArrays);
    }

    // Reading from CSV for the label to AAL_ID map
    CSVReader reader("out.csv");
 
    // Get the data from CSV File
    std::vector<std::vector<std::string>> dataList = reader.getData();
   
    // Print the content of row by row on screen
    for(std::vector<std::string> vec : dataList)
    {
        double key;
        double val;
        int i = 0;

        for(std::string data : vec)
        {
            if (i == 0) {
                key = std::stod(data);
            } else if (i == 1) {
                val = std::stod(data);
            }

            i++;
        }

        if (debug) {
            printf("Key : %lf, Val : %lf\n", key, val);
        }
        m[key] = val;
    }

    // Update scalar
    polydata->GetPointData()->SetActiveScalars(scalarName);
    vtkDoubleArray *scalarData = (vtkDoubleArray*) polydata->GetPointData()->GetArray(scalarName);

    if (scalarData) {
        for (int i = 0; i < nPoints; i++) {
            if (m.find(scalarData -> GetComponent(i, 0)) != m.end()) {
                double updateVal = m[scalarData -> GetComponent(i, 0)];
                if(debug) {
                    printf("%lf\n", updateVal);
                }
                scalarData -> SetComponent(i, 0, updateVal);
            } else if(debug) {
                printf("Data Not Found for : %lf\n", scalarData -> GetComponent(i, 0));
            }
        }
    }

    // Write the new file
    vtkSmartPointer<vtkPolyDataWriter> SurfaceWriter = vtkSmartPointer<vtkPolyDataWriter>::New();
    SurfaceWriter->SetInputData(polydata);
    SurfaceWriter->SetFileName(outputFilename);
    SurfaceWriter->Update();

    return EXIT_SUCCESS;
}