// classes example
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <cstdlib>

using namespace std;


//**********************************************************************************
// CELL class and methods
//**********************************************************************************
// defining Cell class interface
//    -> functions and properties of the class Cell 
class Cell {
	int spin;
   public:
	void set_spin(int);	
	int get_spin();
	void add_spin(int);
};

// defining Cell class methods
//   method to save/set the spin value
void Cell::set_spin (int x) {
	spin = x;
}

//   method to get the spin value
int Cell::get_spin () {
	return spin;
}

//   method to add a value to the actual spin value
void Cell::add_spin(int x) {
	spin = spin + x;
}

//**********************************************************************************
// GRID class and methods
//**********************************************************************************
class Grid
{
   private:
     size_t rows;
	 size_t cols;	
     std::vector<std::vector<Cell> > matrix;
   public:
     Grid(size_t dim_row, size_t dim_col);
	 Cell get_cell(size_t dim_row, size_t dim_col);
     void add_cell_spin(size_t dim_row, size_t dim_col, int value);
};

// CONSTRUCTOR
Grid::Grid(size_t dim_row, size_t dim_col) : rows(dim_row), cols(dim_col), matrix()
{
	matrix.resize(cols);
	for ( size_t i = 0; i < rows; ++i )
		matrix[i].resize(rows);

	// INITIALIZING the matrix with random values
	// creating the cell objects 5x5 with random spin
	// and saving the cell objects in different position of the grid object
	for(size_t i =0; i < cols; i++ ) {
		for(size_t j =0; j < rows; j++ ) {
			// generating random in the domain [1,4] (Piles are stable yet, spin <= 4)  (http://www.cplusplus.com/reference/cstdlib/rand/)
			//int random_spin = rand() % 4 +1;	
			// creating a cell object that will contain the spin and other cell properties	
			Cell gridCell_object = Cell();
			// setting the spin of the gridCell_object
			gridCell_object.set_spin(0);
			// we; save the cell object into the grid matrix to be able to access the spin value later
			//matrix[i * cols + j] = gridCell_object;
			matrix[i][j] = gridCell_object;
		}
	}
}

// defining Grid class methods
//   method get the cell in a location of the matrix
Cell Grid::get_cell(size_t dim_row, size_t dim_col)
{
	return matrix[dim_col][dim_row];
}

// defining Grid class methods
//   method to add a value to the actual spin value of the cell directly
void Grid::add_cell_spin(size_t dim_row, size_t dim_col, int value)
{
	matrix[dim_col][dim_row].add_spin(value);
}
















//**********************************************************************************
// main function
//**********************************************************************************
int main ( int argc, char* argv[] ) {

	// Begin Flags
    int Limit_Number_avalanches=1000000;
	size_t num_cols = 1000;
	size_t num_rows = 1000;
	unsigned x_drop = 500;
	unsigned y_drop = 500;

	cout << endl ;	
    int c ;
    unsigned lx ;
    unsigned ly ;
    while( ( c = getopt (argc, argv, "n:x:y:h:") ) != -1 ) 
    {
        switch(c)
        {
			case 'n':
                if(optarg) {
                Limit_Number_avalanches = std::atoi(optarg) ;
                cout << "  * Flag -n set to " << Limit_Number_avalanches << endl ;
                }
                break;
           	case 'x':
                if(optarg) { 
                lx = std::atoi(optarg) ;
                size_t num_cols = lx ;
                cout << "  * Flag -x size set to " << lx << endl ;
                }
                break;
			case 'y':
                if(optarg) {
                ly = std::atoi(optarg) ;
                size_t num_rows = ly ;
				cout << "  * Flag -y size set to " << ly << endl ;
                }
                break;
			case 'h':
				cout<<"-----------------"<<endl;
				cout<<"Quick help guide:"<<endl;
				cout<<"-----------------"<<endl;
				cout<<endl;
				cout<<"Usage: ./a.out [-option] "<<endl;
				cout<<"option:  "<<"-h  show help information"<<endl;
				cout<<"         "<<"-n  Number of avalanches to be generated (default Naval=100)"<<endl;
				cout<<"         "<<"-x  To set linear size Lx (default Lx=100)"<<endl;
				cout<<"         "<<"-y  To set linear size Ly (default Ly=100)"<<endl;
				cout<<endl;
				cout<<"example:  ./a.out -n 1000 -x 150 -y 150"<<endl;
				cout<<"This will generate 1000 avalanches in a system of size 150X150"<<endl;
				cout<<endl;
				return EXIT_SUCCESS;
        }
    }
	// End Flags


	// Count execution time
	int start_s=clock();
	
	
	// Deterministic sandpile
	cout << endl;
	cout << " \033[1m 2D Random-Rain Sandpile Model\033[0m" << endl;  //;31m
	cout << endl;
	
	
  	cout << " \033[1;34m Droping point: Random\033[0m\n";
  	cout << " \033[1;34m Size: Lx=" << num_cols << "    " << "Ly=" << num_rows << "    Number of sites: " << num_cols*num_rows << "\033[0m" << endl;
  	cout << " \033[1;34m Number of simulated avalanches: " << Limit_Number_avalanches <<"\033[0m\n";
  	cout << endl;
  	
  	cout << "\r" << "\033[1;31m  -> Initializing sandpile... \033[0m" << flush ;
  	// creating the matrix object "grid" that will contain the cell_objects
	Grid grid_object(num_rows,num_cols);
	cout << "\033[1;32m DONE\033[0m\n";

  	// creating vectors to store positions of the unstable spins THIS COULD BE A PROPERTY OF THE CELL.
  	size_t unstable_vector_size = num_cols*num_rows;    
    std::vector<int> ux(unstable_vector_size), uy(unstable_vector_size);
    
    // Vector to store phase avalanche size distribution.
    size_t Phase_Avalanche_Vector_Size = 1000*num_cols*num_rows;
	std::vector<int> D_phase_Size(Phase_Avalanche_Vector_Size,0); // initializes the vector to zerio directly
	// Vector to store phase avalanche duration distribution.
	size_t Phase_Avalanche_Vector_Duration = num_cols*num_rows;
	std::vector<int> D_phase_Duration(Phase_Avalanche_Vector_Duration,0); // initializes the vector to zerio directly
	
	//cout << "\r" << "\033[1;31m  -> Generating avalanches... \033[0m" << flush ;
	
	int Avalanche_number = 0;
	
	
	while(Avalanche_number < Limit_Number_avalanches) {
	
		label2:

		// Selecting a random grid cell and adding a sand grain to the cell allocated at this grid position. Also prints the change before and after.
		//int random_row = rand()%num_rows;	
		//int random_col = rand()%num_cols;
		//cout << "Changing spin in cell associated to grid position: i= " << random_col << "  " << "j= " << random_row << "\n";
		//cout << "Before adding 1 spin=" <<  grid_object.get_cell(random_col,random_row).get_spin();
		grid_object.add_cell_spin(x_drop,y_drop,1); // 501 -> 250  701->350
		//cout << ". After adding 1 spin=" <<  grid_object.get_cell(random_col,random_row).get_spin() << "\n";
		
		if (grid_object.get_cell(x_drop,y_drop).get_spin() <= 4) {
			goto label2;
		}
		
		// Checking unstabilities of the spins in the cells.
		int Phase_Avalanche_Size = 0;
		int Phase_Avalanche_Duration = 0;
		
		label1:
		
		int unstabilities_index = 0;
		
		for(int i =0; i < num_cols; i++ ) {
			for(int j =0; j < num_rows; j++ ) {
				// If satifies this condition the spin is unstable and we save the position of the unstable spin.
				if (grid_object.get_cell(i,j).get_spin() > 4) {
					//cout << "Unstable Spin= " <<  grid_object.get_cell(i,j).get_spin() << "    Iteration = " << k << "\n";
					++unstabilities_index;
					ux[unstabilities_index]=i;
					uy[unstabilities_index]=j;
				}
			}
		}
		//cout << "-> Unstable at iteration k= " << k << " Number of unstabilities=" << unstabilities_index << "\n";
		
		// If unstabilities_index > 0 means there are spins to be equilibrated
		// Equilibration of the unstable spins:
		if (unstabilities_index > 0) {
		
			// Begin r loop over unstable positions
			for(int r =1; r < unstabilities_index+1; r++ ) {
				//The unstable spin is decreased by 4:
				grid_object.add_cell_spin(ux[r], uy[r],-4);
				// First neighbours receive the grains of the unstable spin.
				// Open boundary conditions considered here.
				
				if (ux[r]+1 < num_cols) {
					grid_object.add_cell_spin(ux[r]+1, uy[r],1);	
				}
				
				if (ux[r]-1 >= 0 ) { 
					grid_object.add_cell_spin(ux[r]-1, uy[r],1);	
				}
				
				if (uy[r]+1 < num_rows) {
					grid_object.add_cell_spin(ux[r], uy[r]+1,1);	
				}
				
				if (uy[r]-1 >= 0 ) { 
					grid_object.add_cell_spin(ux[r], uy[r]-1,1);	
				}
			}
			// End r loop over unstable positions
			
			Phase_Avalanche_Size = Phase_Avalanche_Size + unstabilities_index;
			++Phase_Avalanche_Duration;
			
			goto label1;
			
		}// if unstabilities_index
		
		if (Phase_Avalanche_Size > 0) {
			//cout << "Detected phase avalanche of size = " << Phase_Avalanche_Size << "\n";
			++D_phase_Size[Phase_Avalanche_Size];
			++Avalanche_number;
		}
		
		if (Phase_Avalanche_Duration > 0) {
			//cout << "Detected phase avalanche of duration = " << Phase_Avalanche_Size << "\n";
			++D_phase_Duration[Phase_Avalanche_Duration];
			//cout << "Duration: " << Phase_Avalanche_Duration << "\n";
		}
		
		cout << "\033[1;31m     Avalanche number: " << Avalanche_number << "\033[0m\r"; //<< "      Size = " << Phase_Avalanche_Size << "      Duration = " << Phase_Avalanche_Duration << "\033[0m\r";
		cout.flush();
	}// while iteration
	
	cout << "\033[1;31m  -> Generating avalanches ... \033[0m" << flush;
	cout << "\033[1;32m DONE\033[0m" << endl;
	
	//cout << "\033[1;32m DONE\033[0m\n";
	
	
	// BEGIN Save phase avalanche size distribution
 	cout << "\033[1;31m  -> Saving phase avalanche size distribution ... \033[0m" << flush;
 	ofstream PAS_distribution;
 	PAS_distribution.open ("Phase_avalanche_size_distribution.dat", ios::out | ios::trunc); // app=append
 		PAS_distribution << "#! Phase avalanche size distribution: \n";
 		PAS_distribution << "#! Size		Frequency \n";
		for (int i=1; i<Phase_Avalanche_Vector_Size; i++) {
			if ( D_phase_Size[i] > 0) {
				PAS_distribution << i << "  " << D_phase_Size[i] << "\n";
			}
		}
 	PAS_distribution.close();
 	cout << "\033[1;32m DONE\033[0m" << endl;
 	// END Save phase avalanche size distribution
 	
 	
 	// BEGIN Save phase avalanche duration distribution
 	cout << "\033[1;31m  -> Saving phase avalanche duration distribution ... \033[0m" << flush;
 	ofstream PAD_distribution;
 	PAD_distribution.open ("Phase_avalanche_duration_distribution.dat", ios::out | ios::trunc); // app=append
 		PAD_distribution << "#! Phase avalanche duration distribution: \n";
 		PAD_distribution << "#! Duration		Frequency \n";
		for (int i=1; i<Phase_Avalanche_Vector_Duration; i++) {
			if ( D_phase_Duration[i] > 0) {
				PAD_distribution << i << "  " << D_phase_Duration[i] << "\n";
//				cout << i << "  " << D_phase_Duration[i] << "\n";
			}
		}
 	PAS_distribution.close();
 	cout << "\033[1;32m DONE\033[0m" << endl;
 	// END Save phase avalanche duration distribution
 	
 	
 	// BEGIN Save sandpile configuration picture in pgm format
	cout << "\033[1;31m  -> Generating pgm figure of the initial configuration... \033[0m" << flush;
	ofstream PBM_picture;
	PBM_picture.open ("BTW_RAN.pgm", ios::out | ios::trunc); //| ios::app | ios::binary
		PBM_picture << "P3\n";
	
		PBM_picture << num_cols << "\n";
		PBM_picture << num_rows << "\n";
		PBM_picture << "255" << "\n";

		for (int i=0; i<num_cols; i++) {
			for (int j=0; j<num_rows; j++) {
    		
    		    int value = grid_object.get_cell(i,j).get_spin();
    		
				if ( value == 0 ) {
      				PBM_picture << "  255    255    255\n";
	 			}
				if ( value == 1 ) {
    	   			PBM_picture << "233  246   49\n";	
    			}
    	  		if ( value == 2) {
    	   			PBM_picture << "246   75   49\n";
    	  		}
    	 		if ( value == 3) {
		        	PBM_picture << " 31  128   11\n";
   	    		}
   	    		if ( value == 4) {
   	    			PBM_picture << " 53  110  223\n";
   	    		}
			}
		}
 	PBM_picture.close();
 	cout << "\033[1;32m DONE\033[0m" << endl;
 	
	// End counting time 	
 	int stop_s=clock();
 	
 	cout << endl;
 	cout << "  Number of avalanches: \033[1m" << Avalanche_number << "\033[0m" << endl;
	cout << "  Execution time: \033[1m" << (stop_s-start_s)/double(CLOCKS_PER_SEC) << "\033[0m seconds" << endl;
	cout << endl;

 	// END Save a nice picture in pgm format
}

