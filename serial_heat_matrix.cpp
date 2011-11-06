//Arvind
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct ProblemInfo {
	int numberOfNodesX;
	int numberOfNodesY;
	double endSimulationTime;
	double thermalDiffusivity;
	double CFL;
};

bool processUserInput(ProblemInfo &theProblemInfo);

//for border conditions
void setupBoundaryConditions(double** theArray, int arraySizeX, int arraySizeY);

//extract the tile temperatures
void initializeInteriorNodes(double** theArray, int arraySizeX, int arraySizeY);
void initializeArray(double** theArray, int arraySizeX, int arraySizeY);

double** make2DDoubleArray(int arraySizeX, int arraySizeY);
//output file
void saveToFile(ProblemInfo theProblemInfo, double** theData, char* theFileName);


//main begins
int main(int argc, char* argv[]) {

	int nNodesX,
		nNodesY;

	int ix = 0,
		jy =0;

	double endSimulationTime = 1.0, 
		CFL = 0.25, 
		deltaT,
		dx,
		dy,
		dx2,
		dy2;

	clock_t startCPUTime = 0,
		endCPUTime = 0, 
		totalCPUTime = 0;

	ProblemInfo probInfo;

	/* process user input */
	if (!processUserInput(probInfo)) return 0; //finish the code

	//Following is to evaluate the deltaT to emulate simulation
	//**********
	nNodesX = probInfo.numberOfNodesX;
	nNodesY = probInfo.numberOfNodesY;	
	dx =(double) 1.0/probInfo.numberOfNodesX;
	dy =(double) 1.0/probInfo.numberOfNodesY;
	dx2 = dx*dx;
	dy2 = dy*dy;

	CFL = probInfo.CFL;
	deltaT = CFL*(dx2*dy2)/((dx2+dy2)*probInfo.thermalDiffusivity);
	//***********
	

	/* memory allocation */		
	double** uOld = make2DDoubleArray(nNodesX,nNodesY);	
	initializeArray(uOld,nNodesX,nNodesY);

	double** uNew = make2DDoubleArray(nNodesX,nNodesY);
	setupBoundaryConditions(uNew,nNodesX,nNodesY);

	startCPUTime=clock();

	int iter = 0;
	while(iter++*deltaT <= probInfo.endSimulationTime) {
		/* calculate the new values of the dependant variable; 5 point stencil */
		for (ix = 1; ix < (nNodesX-1); ix++) {
			for (jy = 1; jy < (nNodesY-1) ;jy++){
				uNew[ix][jy] = uOld[ix][jy] + CFL*(uOld[ix+1][jy] + uOld[ix-1][jy] - 4*uOld[ix][jy] + uOld[ix][jy-1] + uOld[ix][jy+1]);
			}
		}

		/* set uOld = uNew */
		for (ix = 1; ix < (nNodesX-1); ix++) {
			for (jy = 1; jy < (nNodesY-1) ;jy++){
				uOld[ix][jy] = uNew[ix][jy];
			}
		}
	}

	endCPUTime = clock();

        /* compute the total wall time */
        totalCPUTime= endCPUTime-startCPUTime;

        /* compute the maximum wall time */
        printf("Time to execute %lf s\n",(double) totalCPUTime/CLOCKS_PER_SEC);


	saveToFile(probInfo,uNew,"2DHeatEquation.dat");

	return 0;
}//main ends


bool processUserInput(ProblemInfo &theProblemInfo) {

	printf("Enter the number of rows:");
	fflush(stdout);
	scanf("%d",&theProblemInfo.numberOfNodesX);	

	printf("Enter the number of columns:");
	fflush(stdout);
	scanf("%d",&theProblemInfo.numberOfNodesY);	

	if (theProblemInfo.numberOfNodesX == 0 || theProblemInfo.numberOfNodesY==0) return 0;
	printf("Enter thermalDiffusivity:");
	fflush(stdout);
	scanf("%lf",&theProblemInfo.thermalDiffusivity);		

	theProblemInfo.endSimulationTime=2.0;		

	printf("Enter the Courant-Friedrichs-Lewy (CFL) number\n");
	fflush(stdout);
	scanf("%lf",&theProblemInfo.CFL);

	return 1;
}

void setupBoundaryConditions(double** theArray,int arraySizeX, int arraySizeY) {
	int ix, jy;
	double leftBC = 27, 
		rightBC = 27, 
		topBC = 27, 
		bottomBC = 27;

	// setup the bottom and top BCs, jy = 0 and jy = n-1 or arraySizeY - 1
	// we fill this values based on the get values of the adjoining slices
	for (ix = 0; ix < arraySizeX; ix++) {
		theArray[ix][0] = bottomBC;		//bottom BC
		theArray[ix][arraySizeY-1] = topBC;	//top BC
	}	

	// setup the left and right BCs, ix = 0 and ix = arraySizeX - 1
	for (jy = 0; jy < arraySizeY; jy++) {
		theArray[0][jy] = leftBC;	//left BC
		theArray[arraySizeX-1][jy] = rightBC;	//right BC
	}

	// set the values at the corner nodes as averages of both sides
	// bottom left
	theArray[0][0] = (leftBC + bottomBC)*0.5;
	// top left
	theArray[0][arraySizeY-1] = 0.5*(topBC + leftBC);
	// top right
	theArray[arraySizeX-1][arraySizeY-1] = 0.5*(topBC + rightBC);
	// bottom right
	theArray[arraySizeX-1][0] = 0.5*(bottomBC + rightBC);
}

void initializeInteriorNodes(double** theArray, int arraySizeX, int arraySizeY) {
	int ix, jy;
	int count_line, count_token;
	int i;		
	
	for (ix = 1; ix <= arraySizeX - 2; ix++) {
		for (jy = 1; jy <= arraySizeY - 2; jy++) {
			theArray[ix][jy] = 47.00;
		}
	}
}

void initializeArray(double** theArray, int arraySizeX, int arraySizeY) {
	setupBoundaryConditions(theArray,arraySizeX,arraySizeY);
	initializeInteriorNodes(theArray,arraySizeX,arraySizeY);	
}

double** make2DDoubleArray(int arraySizeX, int arraySizeY) {
	double** theArray;
	theArray = (double**) malloc(arraySizeX*sizeof(double*));
	for (int ix = 0; ix < arraySizeX; ix++) {
		theArray[ix] =(double*) malloc(arraySizeY*sizeof(double));
	}
	return theArray;
}


void saveToFile(ProblemInfo theProblemInfo, double** theData, char* theFileName) {
	int nNodesX = theProblemInfo.numberOfNodesX;
	int nNodesY = theProblemInfo.numberOfNodesY;
	double dx =(double) 1.0/theProblemInfo.numberOfNodesX;
	double dy =(double) 1.0/theProblemInfo.numberOfNodesY;
	FILE* theDataFile;
	theDataFile = fopen(theFileName,"w");
	for (int ix = 0; ix < nNodesX; ix++) {
		for (int jy = 0; jy < nNodesY;jy++){
			fprintf(theDataFile,"%lf",theData[ix][jy]);
		}
		fprintf(theDataFile,"\n");
	}
	fclose(theDataFile);
}
