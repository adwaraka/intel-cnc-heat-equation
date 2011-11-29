// Arvind
// CnC/TBB headers to be added
// typedef std::pair<int,int> pair;

#include "cnc/cnc.h"
using namespace CnC;

struct MatrixKey{
    int x, y, time;

    MatrixKey(int x0, int y0, int time0) : 
    x(x0), y(y0), time(time0) {};

    // Overloading the == operator
    bool operator==( const MatrixKey & t ) const {
        return  x == t.x && y == t.y && time == t.time;
    }

};

template <>
class cnc_tag_hash_compare< MatrixKey > {
  public:
    size_t hash(const MatrixKey& t) const
    {
        int h = t.x;
	/*         
        unsigned int high1 = h & 0xf8000000;
        h = h << 5;
        h = h ^ (high1 >> 27);
        h = h ^ t.y;

        unsigned int high2 = h & 0xf8000000;
        h = h << 5;
        h = h ^ (high2 >> 27);
        h = h ^ t.time;
	*/
        return size_t(h);
    }
        
    bool equal(const MatrixKey& a, const MatrixKey& b) const { return a == b; }
};

std::ostream & cnc_format( std::ostream& os, const MatrixKey &m )
{
	return os <<m.x << "," << m.y << "," << m.time;
}


struct ProblemInfo {
	int numberOfNodesX;
	int numberOfNodesY;
};

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>

#include "heat_matrix.h"

// LIST OF FUNCTIONS
bool processUserInput(ProblemInfo &theProblemInfo);
void saveToFile(ProblemInfo theProblemInfo, heat_matrix_context & c, char* theFileName);


// COMPUTE FUNCTION
int compute_heat::execute(const MatrixKey & t, heat_matrix_context & c ) const
{
	int row, col, time_key;

	MatrixKey mat( row, col, time_key );
	row=t.x;
	col=t.y;
	time_key=t.time;


	// For each input item for this step retrieve the item using the proper tag value
	float prev_temp_val,
	    prev_temp_val_l,
	    prev_temp_val_r,
	    prev_temp_val_t,
	    prev_temp_val_b;

        // get the neighbouring data
	c.matrix_value.get(MatrixKey(row,col,time_key), prev_temp_val);
   	c.matrix_value.get(MatrixKey(row-1,col,time_key), prev_temp_val_l);
	c.matrix_value.get(MatrixKey(row+1,col,time_key), prev_temp_val_r);
	c.matrix_value.get(MatrixKey(row,col-1,time_key), prev_temp_val_b);
	c.matrix_value.get(MatrixKey(row,col+1,time_key), prev_temp_val_t);

		
	// printf("Computing for time %d\n",time_key);

	// Step implementation logic goes here	
	// For each output item for this step, put the new item using the proper tag value   
	// sum all the neighbouring ones with the concerned pixel

	c.matrix_value.put(MatrixKey(row, col, time_key+1), 
		float(prev_temp_val + 0.20*(prev_temp_val_l + prev_temp_val_r - 4*prev_temp_val + prev_temp_val_t + prev_temp_val_b)));

	return CnC::CNC_Success;
}


// MAIN FUNCTION IS CALLED
int main(int argc, char* argv[]) {
	heat_matrix_context c;

	int nNodesX, nNodesY;
	int ix = 0, jy =0;
	double  deltaT,
		dx,
		dy,
		dx2,
		dy2;

	int i;
	int j;
	int t;

	ProblemInfo probInfo;
	double CFL;	

	if (!processUserInput(probInfo)) return 0; //finish the code

	nNodesX = probInfo.numberOfNodesX;
	nNodesY = probInfo.numberOfNodesY;	
	dx =(double) 1.0/probInfo.numberOfNodesX;
	dy =(double) 1.0/probInfo.numberOfNodesY;
	dx2 = dx*dx;
	dy2 = dy*dy;

	CFL = 0.20;
	deltaT = CFL*(dx2*dy2)/((dx2+dy2)*0.0008418);

	printf("Initialize the temperatures\n");

    	// Put initial temperatures of 47.00 Degrees Celsius at the center
    	for (i = 1; i < nNodesX-1; i++) {
		for (j = 1; j < nNodesY-1; j++) {
	        	c.matrix_value.put(MatrixKey(i,j,0), float(47.00));
		}
    	}

	printf("Initialize the boundary conditions\n");

	for (i = 0; i < nNodesX; i++) {
		c.matrix_value.put(MatrixKey(i,0,0), 27.00);	//bottom BC
		c.matrix_value.put(MatrixKey(i,nNodesY-1,0), float(27.00));	//top BC
	}

	for (j = 0; j < nNodesY; j++) {
		c.matrix_value.put(MatrixKey(0,j,0), 27.00);	//left BC
		c.matrix_value.put(MatrixKey(nNodesX-1,j,0), float(27.00));	//right BC
	}


        // Initiate computation by putting tags into a tag collection:
        // for (...)
    	for (i = 1; i < nNodesX-1; i++) {
            for (j = 1; j < nNodesY-1; j++) {
		for(t = 0; t <= 169; t++)  {
			c.position.put(MatrixKey(i,j,t));
		}
	    }
	}

	printf("Done initializing.... \n");

	// compute each step till end of simulation
	// int iter = 0;
	// while(iter++*deltaT <= double(2.0)) {
	// }


	c.wait();

	// writing to file
	printf("Writing to the file\n");
	saveToFile(probInfo, c,"2DHeatEquation_parallel.dat");

}
//main ends


// FUNCTIONS
bool processUserInput(ProblemInfo &theProblemInfo) {
	printf("Enter the number of rows:");
	fflush(stdout);
	scanf("%d",&theProblemInfo.numberOfNodesX);	

	printf("Enter the number of columns:");
	fflush(stdout);
	scanf("%d",&theProblemInfo.numberOfNodesY);	

	return 1;
}

void saveToFile(ProblemInfo theProblemInfo, heat_matrix_context & c, char* theFileName) {
	int nNodesX = theProblemInfo.numberOfNodesX;
	int nNodesY = theProblemInfo.numberOfNodesY;
	float value;

	FILE* theDataFile;
	theDataFile = fopen(theFileName,"w");
	for (int ix = 0; ix < nNodesX; ix++) {
		for (int jy = 0; jy < nNodesY; jy++){
			c.matrix_value.get(MatrixKey(ix, jy, 169), value);
			fprintf(theDataFile,"%lf",value);			
		}
		fprintf(theDataFile,"\n");
	}
	fclose(theDataFile);
}
