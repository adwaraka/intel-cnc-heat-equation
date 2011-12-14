// Arvind
// CnC/TBB headers to be added
// typedef std::pair<int,int> pair;

#include "cnc/cnc.h"
#include "cnc/debug.h"
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
        return size_t(h);
    }
        
    bool equal(const MatrixKey& a, const MatrixKey& b) const { return a == b; }
};

std::ostream & cnc_format( std::ostream& os, const MatrixKey &m )
{
	return os << m.x << "," << m.y << "," << m.time;
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

int MAX_ROW, MAX_COL;

// LIST OF FUNCTIONS
bool processUserInput(ProblemInfo &theProblemInfo);
//void saveToFile(ProblemInfo theProblemInfo, heat_matrix_context & c, char* theFileName);


// COMPUTE FUNCTION
int compute_heat::execute(const MatrixKey & t, heat_matrix_context & c ) const
{
	int row, col;
	int time_key;

	row=t.x;
	col=t.y;
	time_key=t.time;

	// For each input item for this step retrieve the item using the proper tag value
	float prev_temp_val,
	    prev_temp_val_l,
	    prev_temp_val_r,
	    prev_temp_val_t,
	    prev_temp_val_b;

	float value;	

	// printf("Max Row, Col : %d %d\n", MAX_ROW, MAX_COL);		
        MatrixKey mat(row, col, t.time+1);

	if (((row == 0)||(row == MAX_ROW-1)||(col == 0) ||(col == MAX_COL-1)) && time_key >= 0) {
		c.matrix_value.put(mat, float(27.00));
	}
	else 
	{
		c.matrix_value.get(MatrixKey(row,col,time_key), prev_temp_val);
  		c.matrix_value.get(MatrixKey(row-1,col,time_key), prev_temp_val_t);
		c.matrix_value.get(MatrixKey(row+1,col,time_key), prev_temp_val_b);
		c.matrix_value.get(MatrixKey(row,col-1,time_key), prev_temp_val_l);
		c.matrix_value.get(MatrixKey(row,col+1,time_key), prev_temp_val_r); 
	
		printf("Computing for row, column and time's current values - %d, %d, %d \n", row, col, time_key);
		printf("Current Values : %f\n", prev_temp_val);
		
		// printf("Next time value: %d\n", mat.time);
		printf("Computing for row, column and time - %d, %d, %d \n", mat.x, mat.y, mat.time);
		c.matrix_value.put(mat, float(prev_temp_val + 0.20*(prev_temp_val_l + prev_temp_val_r - 4*prev_temp_val + prev_temp_val_t + prev_temp_val_b)));
       		c.matrix_value.get(mat, value);
		printf("New Value : %f\n", value);	
	}

    	CnC::debug::trace( c.position, "position" );
	CnC::debug::trace( c.matrix_value, "matrix_value" );
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
	int t = 0;

	ProblemInfo probInfo;
	double CFL;	

	if (!processUserInput(probInfo)) return 0; //finish the code

        nNodesX = probInfo.numberOfNodesX;
        MAX_ROW = probInfo.numberOfNodesX;
	nNodesY = probInfo.numberOfNodesY;	
	MAX_COL = probInfo.numberOfNodesY;	
	dx =(double) 1.0/probInfo.numberOfNodesX;
	dy =(double) 1.0/probInfo.numberOfNodesY;
	dx2 = dx*dx;
	dy2 = dy*dy;

	CFL = 0.20;
	deltaT = CFL*(dx2*dy2)/((dx2+dy2)*0.0008418);

	printf("The values of x and y are %d and %d\n",nNodesX, nNodesY);


    	// Put initial temperatures of 47.00 Degrees Celsius at the center
    	for (i = 1; i < nNodesX-1; i++) {
		for (j = 1; j < nNodesY-1; j++) {
	        	c.matrix_value.put(MatrixKey(i,j,0), float(47.00));
		}
    	}


	for (i = 0; i < nNodesX; i++) {
		c.matrix_value.put(MatrixKey(i,0,0), 27.00);	//left BC
		c.matrix_value.put(MatrixKey(i,nNodesY-1,0), float(27.00));	//right BC
	}

	for (j = 0; j < nNodesY; j++) {
		c.matrix_value.put(MatrixKey(0,j,0), 27.00);	//top BC
		c.matrix_value.put(MatrixKey(nNodesX-1,j,0), float(27.00));	//left BC
	}


        // Initiate computation by putting tags into a tag collection:
        // for (...)
    	for (i = 0; i <= nNodesX-1; i++) {
            for (j = 0; j <= nNodesY-1; j++) {
	        for (t = 0; t <= 10 ; t++) {
			c.position.put(MatrixKey(i,j,t)); 
		}
	    }
	}

    	// CnC::debug::trace( c.position, "position" );
	c.wait();
}



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

