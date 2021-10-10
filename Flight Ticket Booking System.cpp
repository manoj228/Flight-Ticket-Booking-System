#include<bits/stdc++.h>
#include <dirent.h>
#include <sys\stat.h>
using namespace std;

// used to maintatin businessClass 
struct BusinessClass
{
	vector<string> businessSeats;
	vector<vector<string>> businessRowArrangement;
	vector<vector<vector<char>>> eachBusinessRow, eachBusinessRowCopy;
	vector<int> businessTotalSeats;
};

// used to maintatin economyClass 
struct EconomyClass
{
	vector<string> economySeats;
	vector<vector<string>> economyRowArrangement;
	vector<vector<vector<char>>> eachEconomyRow, eachEconomyRowCopy;
	vector<int> economyTotalSeats;
	unordered_map<int, pair<int,int>> economyBookingInfo; // {bookingId -> {seat rowNum, seat colNum}}
};

// used to maintatin Flight details
struct Flight
{
	int listCount = 0;
	vector<string> flightId, flightNames, flightDetails;
	vector<string> flightSource, flightDestination;
	BusinessClass businessObj;
	EconomyClass economyObj; 	
};

// used to maintatin passenger Info 
struct Passenger
{
	string names, seatNum, passFlightId;
	int ages;
	char isMealOrdered, busOrEcon;
	int bookingId, ticketPrice = 0;
};

unordered_map<int, int> bookIdRef; 	// booking id - index of passenger details
vector<Passenger> passObj;
vector<vector<vector<char>>> businessRow, economyRow, businessRowCopy, economyRowCopy;
Flight flightObj;
map<string, vector<string>> srcToDest;
bool businessFlag = 0, economyFlag = 0;
int row = 0, col = 0, totalPassCount = 0;

// base price of ticket
vector<int> businessClassTicketPrice, economyClassTicketPrice;

// this function used to get position of flight using flight Id
int getIndexOfFlightId(string id)
{
	for(int i=0; i<flightObj.flightSource.size(); i++)
	{
	    string curFlightId = flightObj.flightId[i];

		if(curFlightId == id)
		{
			return i;
		}
	}
	
	return -1;
}

// this function is used to print business class Row Arrangements
void printBusinessClassArrangement(int indexOfGivenId)
{
	cout << "\nBusiness Seats : \n";
	int l = 0, k, size, rowSize, colSize;
	size = flightObj.businessObj.businessRowArrangement[indexOfGivenId].size();
	rowSize = stoi(flightObj.businessObj.businessSeats[indexOfGivenId]);
			
	/*  since we stored seat arrangement in 3d vector, 
	    so while printing we need to give space in correct position (which is stored in businessRowArrangement)
		based on the values , we are decrementing the cur row-col , 
		when it reaches to zero, again update with next val */
	for(auto row : businessRow[indexOfGivenId])
	{	
		l=0, k = stoi(flightObj.businessObj.businessRowArrangement[indexOfGivenId][l]);
		k -=1, colSize = flightObj.businessObj.businessTotalSeats[indexOfGivenId];
		for(auto val : row)
		{
			cout << val << " ";
			if(k--==0)
			{
				cout << " ";
					
				// updating the next position to print space
				k = stoi(flightObj.businessObj.businessRowArrangement[indexOfGivenId][++l]);
					
				// since we are 0-based index, so minus 1
				k -=1;
					
				if(l == size-1) l=0;
				
			} 
			
			//column limit to write file because, we have initialized 3d value with default as 30
			colSize--;			
			if(colSize == 0) break;
		}				
		cout << endl;
		
		//row limit to write file because, we have initialized 3d value with default as 30
	 	rowSize--;
		if(rowSize == 0) break;
	}
}

// this function is used to print economy class Row Arrangements
void printEconomyClassArrangement(int indexOfGivenId)
{
	cout << "\nEconomy Seats : \n";
	int l = 0, k, size, rowSize, colSize;
	vector<vector<char>> eachRow = economyRow[indexOfGivenId]; 
	
	size = flightObj.economyObj.economyRowArrangement[indexOfGivenId].size();
	rowSize = stoi(flightObj.economyObj.economySeats[indexOfGivenId]);
	/*  since we stored seat arrangement in 3d vector, 
	    so while printing we need to give space in correct position (which is stored in businessRowArrangement)
		based on the values , we are decrementing the cur row-col , 
		when it reaches to zero, again update with next val */
	for(auto row : eachRow)
	{	
		l=0,k = stoi(flightObj.economyObj.economyRowArrangement[indexOfGivenId][l]);
		k -=1,colSize = flightObj.economyObj.economyTotalSeats[indexOfGivenId];
		for(auto val : row)
		{
			cout << val << " ";
			if(k--==0)
			{
				cout << " ";

				// updating the next position to print space
				k = stoi(flightObj.economyObj.economyRowArrangement[indexOfGivenId][++l]);
					
				// since we are 0-based index, so minus 1
				k -=1;
				
				if(l == size-1) l=0;
			} 
			
			// column limit to write file because, we have initialized 3d value with default as 30
			colSize--;
			if(colSize == 0) break;
		}				
		cout << endl;
		
		//row limit to write file because, we have initialized 3d value with default as 30
		rowSize--;
		if(rowSize == 0) break;
	}	
}

// it will update the 3d vector using the updated files 
void updateArrangements(int indexOfGivenId, string line)
{
	int size = flightObj.businessObj.businessRowArrangement[indexOfGivenId].size();
	if(businessFlag && !economyFlag)
	{
		col = 0;
		for(auto ch : line)
		{
			if(ch != ' ')
			businessRow[indexOfGivenId][row][col++] = ch;
		}
		++row;
	} 
	else{
		col = 0;
		for(auto ch : line)
		{
			if(ch != ' ')
			economyRow[indexOfGivenId][row][col++] = ch;
		}
		++row;
	}	
}

// it will read all the contents of files inside the given path
void getAllCurrentSeatsFromFolder(int indexOfGivenId)
{
	ifstream file;
	string line;
			
	// str will be folder name (that contains flight available seats) + flight name.txt
	string str =  "./Available Seats/" + flightObj.flightNames[indexOfGivenId];
	file.open(str.c_str());
	
	
	if(file.is_open()){
        while(getline(file, line))
		{
			if(line[0] == 'B'){ row = 0, businessFlag = true; continue; }
			else if(line[0] == 'E'){ row = 0, economyFlag = true; continue; }
			
			if(line.size() > 0)
			updateArrangements(indexOfGivenId, line);	
		}
	}
	
	file.close();
	
	// reset the value
	businessFlag = economyFlag = false;
	row = 0, col = 0;
}

// update the business seats in file for using flight id 
void writeToFileBusiness(ofstream &file,string idFlight, string path)
{
	
	file.open(path.c_str());
		
	int indexOfGivenId = getIndexOfFlightId(idFlight);
	
	if(indexOfGivenId == -1) {
		cout << "Invalid Flight ID";
		return;
	}
	file << "Business Seats\n";
	int l = 0, k, size, rowSize, colSize;
	size = flightObj.businessObj.businessRowArrangement[indexOfGivenId].size();
	rowSize = stoi(flightObj.businessObj.businessSeats[indexOfGivenId]);
			
	/*  since we stored seat arrangement in 3d vector, 
	    so while printing we need to give space in correct position (which is stored in businessRowArrangement)
		based on the values , we are decrementing the cur row-col , 
		when it reaches to zero, again update with next val */
	for(auto row : businessRow[indexOfGivenId])
	{	
		l=0, k = stoi(flightObj.businessObj.businessRowArrangement[indexOfGivenId][l]);
		k -=1, colSize = flightObj.businessObj.businessTotalSeats[indexOfGivenId];
		for(auto val : row)
		{
			file << val;
			if(k--==0)
			{
				file << " ";
					
				// updating the next position to print space
				k = stoi(flightObj.businessObj.businessRowArrangement[indexOfGivenId][++l]);
					
				// since we are 0-based index, so minus 1
				k -=1;
					
				if(l == size-1) l=0;
				
			} 
			
			//column limit to write file because, we have initialized 3d value with default as 30
			colSize--;			
			if(colSize == 0) break;
		}				
		file << endl;
		
		//row limit to write file because, we have initialized 3d value with default as 30
	 	rowSize--;
		if(rowSize == 0) break;
	}	
	
	file.close();
	//printBusinessClassArrangement(indexOfGivenId);
}

// update the economy seats in file for using flight id 
void writeToFileEconomy(ofstream &file,string idFlight, string path)
{
	file.open(path.c_str(), ios::app);
		
	int indexOfGivenId = getIndexOfFlightId(idFlight);
	
	if(indexOfGivenId == -1) {
		cout << "Invalid Flight ID";
		return;
	}
	
	file << "\nEconomy Seats\n";
	int l = 0, k, size, rowSize, colSize;
	vector<vector<char>> eachRow = economyRow[indexOfGivenId]; 
	
	size = flightObj.economyObj.economyRowArrangement[indexOfGivenId].size();
	rowSize = stoi(flightObj.economyObj.economySeats[indexOfGivenId]);
	/*  since we stored seat arrangement in 3d vector, 
	    so while printing we need to give space in correct position (which is stored in businessRowArrangement)
		based on the values , we are decrementing the cur row-col , 
		when it reaches to zero, again update with next val */
	for(auto row : eachRow)
	{	
		l=0,k = stoi(flightObj.economyObj.economyRowArrangement[indexOfGivenId][l]);
		k -=1,colSize = flightObj.economyObj.economyTotalSeats[indexOfGivenId];
		for(auto val : row)
		{
			file << val;
			if(k--==0)
			{
				file << " ";

				// updating the next position to print space
				k = stoi(flightObj.economyObj.economyRowArrangement[indexOfGivenId][++l]);
					
				// since we are 0-based index, so minus 1
				k -=1;
				
				if(l == size-1) l=0;
			} 
			
			// column limit to write file because, we have initialized 3d value with default as 30
			colSize--;
			if(colSize == 0) break;
		}				
		file << endl;
		
		//row limit to write file because, we have initialized 3d value with default as 30
		rowSize--;
		if(rowSize == 0) break;
	}	
	
	file.close();
//	printEconomyClassArrangement(indexOfGivenId);
}

// it will useful to create each set for business/economy class row arrangements
vector<char> printCol(int size, int curCol, int totSize)
{
	vector<char> cur(size);
	
	// base cases
	if(curCol == 0)
	{
		cur[0] = 'W'; 
	}
	if(curCol == totSize-1)
	{
		cur[size-1] = 'W'; 	
	} 
	
	if(size > 1)
	{
		if(curCol > 0) cur[0] = 'A';
		for(int i=1; i<size-1; i++)
		{
			cur[i] = 'M';
		}
	    if(curCol < totSize-1) cur[size-1] = 'A';
	}
	
	return cur;
}

// update 3d vector using given input for businessClass
void businessClassArrangement(int indexOfGivenId)
{
	if(indexOfGivenId == -1) {
		cout << "\nInvalid Flight ID\n";
		return;
	}
	
	int size, rowSize;	
	size = flightObj.businessObj.businessRowArrangement[indexOfGivenId].size();
	rowSize = flightObj.businessObj.businessTotalSeats[indexOfGivenId];
		
	vector<vector<char>> eachSeats;
	for(int i=0; i<size; i++)
	{
		int n;
		n = stoi(flightObj.businessObj.businessRowArrangement[indexOfGivenId][i]);

		vector<char> eachCol = printCol(n, i, size);
		eachSeats.push_back(eachCol);
	}
	
	// copying first row content alone
	vector<char> seats;
	for(int i=0; i<eachSeats.size(); i++)
	{
		for(int j=0; j<eachSeats[i].size(); j++) 
		seats.push_back(eachSeats[i][j]);
	}
	
	for(int i=0; i<stoi(flightObj.businessObj.businessSeats[indexOfGivenId]); i++)
	{	
		int col = 0;
		for(auto val : seats)
		{
			businessRow[indexOfGivenId][i][col++] = val;
		}
	}
}

// update 3d vector using given input for economy Class
void economyClassArrangement(int indexOfGivenId)
{
	if(indexOfGivenId == -1) {
		cout << "\nInvalid Flight ID\n";
		return;
	}
	
	int size, rowSize;	
	size = flightObj.economyObj.economyRowArrangement[indexOfGivenId].size();
	rowSize = flightObj.economyObj.economyTotalSeats[indexOfGivenId];
		
	vector<vector<char>> eachSeats;
	for(int i=0; i<size; i++)
	{
		int n;
		n = stoi(flightObj.economyObj.economyRowArrangement[indexOfGivenId][i]);

		vector<char> eachCol = printCol(n, i, size);
		eachSeats.push_back(eachCol);
	}
	
	vector<char> seats;
	for(int i=0; i<eachSeats.size(); i++)
	{
		for(int j=0; j<eachSeats[i].size(); j++) 
		seats.push_back(eachSeats[i][j]);
	}
	
	
	for(int i=0; i<stoi(flightObj.economyObj.economySeats[indexOfGivenId]); i++)
	{	
		int col = 0;
		for(auto val : seats)
		economyRow[indexOfGivenId][i][col++] = val;
	}
		
	//flightObj.economyObj.eachEconomyRowCopy = flightObj.economyObj.eachEconomyRow;
}

// function to get all files
void getAllFilesInsideFolder()
{
	DIR *dir;
	struct dirent *in;
	string str = "c:\\Users\\manoj\\Desktop\\Zoho Project\\flight details\\";
	if ((dir = opendir (str.c_str())) != NULL) {
	while ((in = readdir (dir)) != NULL) {
		string path = in -> d_name;			
		flightObj.flightDetails.push_back(path);
	}
	closedir (dir);
	} 
	else {
    perror ("");
	}
}

// function to print all information about flight
void printDetails()
{  
   cout << "\nAll Flight Details:\n";
   cout << "\nFlight Id      Flight Source     Flight Destination     Business Class RowCount     Business Class Row Arrangement    Economy Class RowCount     Economy Class Row Arrangement" << endl;
   for(int i=0; i<flightObj.listCount; i++) 
   {
   	
   	string curSource = flightObj.flightSource[i];
	string curDestination = flightObj.flightDestination[i];
	transform(curSource.begin(), curSource.end(), curSource.begin(), ::tolower);
	transform(curDestination.begin(), curDestination.end(), curDestination.begin(), ::tolower);
	// storing values in map to get quick check if flight availble or not
    srcToDest[curSource].push_back(curDestination);
    
   	cout << " " << flightObj.flightId[i]<< setw(20) << flightObj.flightSource[i] << setw(20) << flightObj.flightDestination[i] <<  setw(20) << flightObj.businessObj.businessSeats[i] << setw(28);
	for(auto businessRow : flightObj.businessObj.businessRowArrangement[i]) cout << businessRow << " ";
	cout << setw(30) << flightObj.economyObj.economySeats[i] << setw(28) ;
	for(auto economyRow : flightObj.economyObj.economyRowArrangement[i]) cout << economyRow << " ";
	
	cout << endl;
   }
   
}

// function used to get size of each seat information
void getSeatsClassWise(string seats)
{
	int curBSeats = 0, curESeats = 0;
	if(seats[0] == 'b' || seats[0] == 'B')
	{ 
	    vector<string> curBusinessRow;
		// increment the index until the seat arrangement
	    int curIdx = 0;
	    while(curIdx < seats.size() && seats[curIdx] != '{') curIdx++;
	
	    curIdx++;
		while(curIdx < seats.size())
		{		
			string colArrangement = "";
			while(curIdx < seats.size() && seats[curIdx] != ',' && seats[curIdx] != '}')
			{
				if(seats[curIdx] != '}' || seats[curIdx] != ',')
				colArrangement += seats[curIdx];
			
				curIdx++;				
			} 			
			curIdx++;
			if(colArrangement.size() > 0)
			{
				curBSeats += stoi(colArrangement);
				curBusinessRow.push_back(colArrangement);
			}
			
   		}
   		
   		curBSeats -= stoi(curBusinessRow.back());
   		flightObj.businessObj.businessSeats.push_back(curBusinessRow.back());
   		
   		// deleting total seats which came in loop
   		curBusinessRow.pop_back();
   		flightObj.businessObj.businessRowArrangement.push_back(curBusinessRow);
   		
   		flightObj.businessObj.businessTotalSeats.push_back(curBSeats);
    }
    
    else if(seats[0] == 'e' || seats[0] == 'E')
	{ 
	    vector<string> curEconomyRow;
		// increment the index until the seat arrangement
	    int curIdx = 0;
	    while(curIdx < seats.size() && seats[curIdx] != '{') curIdx++;
	
	    curIdx++;
		while(curIdx < seats.size())
		{		
			string colArrangement = "";
			while(curIdx < seats.size() && seats[curIdx] != ',' && seats[curIdx] != '}')
			{
				if(seats[curIdx] != '}' || seats[curIdx] != ',')
				colArrangement += seats[curIdx];			
				
				curIdx++;
				
			} 			
			curIdx++;
			if(colArrangement.size() > 0)
			{
				curESeats += stoi(colArrangement);
				curEconomyRow.push_back(colArrangement);
			}
			
   		}
   	
   		curESeats -= stoi(curEconomyRow.back());   		
   		
   	 	flightObj.economyObj.economySeats.push_back(curEconomyRow.back());
   		// deleting total seats which came in loop
   	 	curEconomyRow.pop_back();
   	 	flightObj.economyObj.economyRowArrangement.push_back(curEconomyRow);
   	 	
   	 	flightObj.economyObj.economyTotalSeats.push_back(curESeats);
    }
    
}   

// function used to flight id, source and destination
void getFlightInformation(string flightFile)
{	

	// flight information
	string flightUniqueId, flightSrc, flightDst;
	
	// flight file names
	flightObj.flightNames.push_back(flightFile);
	
	int curIdx = 0, flag = 0;
	while(curIdx < flightFile.size())
	{
		string curString = "";
		while(curIdx++ < flightFile.size() && flightFile[curIdx] != '-' && flightFile[curIdx] != '.')
		{
			curString += flightFile[curIdx];		 
		} 
		
		if(!flag) flag = 1;
		else{
			if(flightUniqueId.empty())
			{
				flightUniqueId = curString;
			}
			else if(flightSrc.empty())
			{
				flightSrc = curString;
			}
			else if(flightDst.empty())
			{
				flightDst = curString;
			}
		}	
	} 
	
	flightObj.flightId.push_back(flightUniqueId);
	flightObj.flightSource.push_back(flightSrc);
	flightObj.flightDestination.push_back(flightDst);
	
}

// function used to get all the file information
void getAvailableSeatInformation(string flightFile)
{	
	ifstream sceneFile;
	string line;
		
	// str will be folder name (that contains flight info) + flight name.txt
	string str = "./flight details/" + flightFile;
	sceneFile.open(str.c_str());

	if(sceneFile.is_open()){
        while(getline(sceneFile, line))
		{
			// get seats counts
			getSeatsClassWise(line);	
		}
	}
    
	sceneFile.close();
}

// used to process all the inputs and make inital arrangement for us
void fetchAndWriteFileContents()
{
	for(int i=0; i<flightObj.flightDetails.size(); i++) {
		if(i > 1){	
		getFlightInformation(flightObj.flightDetails[i]);	
		getAvailableSeatInformation(flightObj.flightDetails[i]);	
		
		++flightObj.listCount;	
    }
   }
   
    // resizing the vector
  	businessRow.resize(flightObj.listCount, vector<vector<char>>(30, vector<char>(30)));
  	economyRow.resize(flightObj.listCount, vector<vector<char>>(30, vector<char>(30)));
  	
  	// copy to use it for cancellation ticket
	businessRowCopy.resize(flightObj.listCount, vector<vector<char>>(30, vector<char>(30)));
  	economyRowCopy.resize(flightObj.listCount, vector<vector<char>>(30, vector<char>(30)));
  	
	businessClassTicketPrice.resize(flightObj.listCount, 2000);
	economyClassTicketPrice.resize(flightObj.listCount, 1000);
   // print the available information
   printDetails();
   
   for(int i=0; i<flightObj.listCount; i++)
   {
   		string id = flightObj.flightId[i];

   		int indexOfGivenId = getIndexOfFlightId(id);	
		businessClassArrangement(indexOfGivenId);
   		economyClassArrangement(indexOfGivenId);
   			
   	//	printBusinessClassArrangement(indexOfGivenId);
   	//	printEconomyClassArrangement(indexOfGivenId);
   		
   		// first time writing value to files in available seats folder
   		ofstream file;
   		string path = "c:\\Users\\manoj\\Desktop\\Zoho Project\\Available Seats\\" + flightObj.flightDetails[i+2];
   		
		writeToFileBusiness(file, flightObj.flightId[indexOfGivenId],path);
		writeToFileEconomy(file, flightObj.flightId[indexOfGivenId],path);
   }   
   
   businessRowCopy = businessRow;
   economyRowCopy = economyRow;
}

// used to convert seat number to corresponding row and column number
void convertToSeatNumber(int &rowNum, int &colNum, string seat)
{
	string cur = "";
	int i;
	for(i=0; i<seat.size(); i++)
	{
		if(seat[i] != '_'){
			cur += seat[i];
		}
		
		else{
			rowNum = stoi(cur) - 1;
			break;
		}
	}
	
	
	colNum = (seat[i+1] - 'A');
}

// function to check whether flight is available or not
bool checkForDest(string src, string dest)
{
	bool flag = 0;
	for(int i=0; i<flightObj.listCount; i++)
	{
		string curSource = flightObj.flightSource[i];
		string curDestination = flightObj.flightDestination[i];
		
		transform(curSource.begin(), curSource.end(), curSource.begin(), ::tolower);
	    transform(curDestination.begin(), curDestination.end(), curDestination.begin(), ::tolower);
	    
		if(curSource == src && curDestination.compare(dest) == 0)
		{
			if(!flag) 
			{
				cout << "\nList of Flights from Source to Destination\n";
				cout << "\nFlight Id      Flight Source     Flight Destination     Business Class RowCount     Business Class Row Arrangement    Economy Class RowCount     Economy Class Row Arrangement" << endl;

			}
			
			cout << " " << flightObj.flightId[i]<< setw(20) << flightObj.flightSource[i] << setw(20) << flightObj.flightDestination[i] <<  setw(20) << flightObj.businessObj.businessSeats[i] << setw(28);
			for(auto businessRow : flightObj.businessObj.businessRowArrangement[i]) cout << businessRow << " ";
			cout << setw(30) << flightObj.economyObj.economySeats[i] << setw(28) ;
			for(auto economyRow : flightObj.economyObj.economyRowArrangement[i]) cout << economyRow << " ";
			flag = 1;
			cout << endl;
		} 
	}
   
    return (!flag) ? false : true;
}

// function to print about seat information
void printSeatInfo()
{
	cout << "\nSeats: \t";
	cout << "\n W - Window  M - Middle  A - Aisle N - Seat Allocated";
}

// function to update files 
void writeAndGetFileInfo(int indexOfGivenId)
{
	ofstream file;
    string path = "c:\\Users\\manoj\\Desktop\\Zoho Project\\Available Seats\\" + flightObj.flightDetails[indexOfGivenId+2];
       
    // update the file
	writeToFileBusiness(file, flightObj.flightId[indexOfGivenId], path);
    writeToFileEconomy(file, flightObj.flightId[indexOfGivenId], path);
    	
    // update 3d vector
    getAllCurrentSeatsFromFolder(indexOfGivenId);
}

// function to update information in files while booking
void updateSeatInCorrespondingFlight(string id, string seatNumber, char businessOrEconomy, int &totalAmount)
{
	int rowNum = -1, colNum = -1;
	convertToSeatNumber(rowNum, colNum, seatNumber);
	
	int indexOfGivenId = getIndexOfFlightId(id);
	
	// booking id will be based on rand()
	int bookingId = rand() % 999999999 + 1000;
	if(indexOfGivenId == -1) {
		cout << "Invalid Flight ID";
		return;
	}
	
	if(businessRow[indexOfGivenId][rowNum][colNum] == 'N' || economyRow[indexOfGivenId][rowNum][colNum] == 'N')
	{
		cout << "\nSeat Already Allocated";
		return;
	}
	
	string passName;
	int passAge;
	char mealOrder;
	cout << "\n Enter name of the passenger : ";
	cin.ignore();
	getline(cin, passName);

	
	cout << "\n Enter age of the passenger : ";
	cin >> passAge;
    
	cout << "\n Do you want meal for this Seat (Y/N) ? ";
	cin >> mealOrder;
	
	Passenger curPassObj;
    curPassObj.names = passName;
    curPassObj.ages = passAge;
    curPassObj.seatNum = seatNumber;
    curPassObj.isMealOrdered = mealOrder;
    curPassObj.busOrEcon = businessOrEconomy;
    curPassObj.bookingId = bookingId;
    curPassObj.passFlightId = id;	
	bookIdRef[bookingId] = totalPassCount++;
	
	// meal cost 200
	if(mealOrder == 'Y' || mealOrder == 'y')
	curPassObj.ticketPrice += 200;
	
	/* B - business*/
	if(businessOrEconomy == 'b' || businessOrEconomy == 'B')
	{
		businessRowCopy[indexOfGivenId][rowNum][colNum] = businessRow[indexOfGivenId][rowNum][colNum] ;
		
		// window and aisle seat costs extra 100
		if(businessRow[indexOfGivenId][rowNum][colNum] == 'W' || businessRow[indexOfGivenId][rowNum][colNum] == 'A')
		curPassObj.ticketPrice += 100;
		
    	businessRow[indexOfGivenId][rowNum][colNum] = 'N';
    	printBusinessClassArrangement(indexOfGivenId);    	
    	writeAndGetFileInfo(indexOfGivenId);
    	
    	// updating ticket price
    	curPassObj.ticketPrice += businessClassTicketPrice[indexOfGivenId];
    	
    	//surge pricing
    	businessClassTicketPrice[indexOfGivenId] += 200;
    	
    	
	}
	else if(businessOrEconomy == 'e' || businessOrEconomy == 'E')
    {
    	economyRowCopy[indexOfGivenId][rowNum][colNum] = economyRow[indexOfGivenId][rowNum][colNum];
    	
    	// window and aisle seat costs extra 100
    	if(economyRow[indexOfGivenId][rowNum][colNum] == 'W' || economyRow[indexOfGivenId][rowNum][colNum] == 'A')
    	curPassObj.ticketPrice += 100;
    	
    	economyRow[indexOfGivenId][rowNum][colNum] = 'N';
    	printEconomyClassArrangement(indexOfGivenId);    
    	writeAndGetFileInfo(indexOfGivenId);   
		
		// updating ticket price
    	curPassObj.ticketPrice += economyClassTicketPrice[indexOfGivenId];
		
		// surge pricing 	
    	economyClassTicketPrice[indexOfGivenId] += 100;
	} 
    else{
    	cout << "Invalid Option";
    	return;
	}
	
	totalAmount += curPassObj.ticketPrice;
	
	cout << "\nKindly Please note of the below booking Id for your reference \n";
	cout << "\nYour Booking Id for this seat : \n" << bookingId;
	
	// inserting into vector
	passObj.push_back(curPassObj);
	
}

// it lists all the files and its count
void listFlight()
{
	cout << "\nNumber of Flights : " << flightObj.flightSource.size() << endl;
	cout << "\nName of Flights " << endl;
	for(int i=0; i<flightObj.flightSource.size(); i++) cout << flightObj.flightNames[i] << endl;
}

// used to search if flight is available or not
bool searchFlight()
{
	string source, destination;
	cout << "\nEnter location from which you are travelling : " ;
	cin >> source;
	cout << "\nEnter location from you are going to travel : ";
	cin >> destination;
	
	// convert input to lower case, so it will be easy to compare
	transform(source.begin(), source.end(), source.begin(), ::tolower);
	transform(destination.begin(), destination.end(), destination.begin(), ::tolower);

	if(srcToDest.find(source) == srcToDest.end())
	{
		cout << "\nNo flights available from source now, you can check back Later!!";
		return false;
	}
	else{
		// flight is available from src, so search for destination
		if(!checkForDest(source, destination))
		{
			cout << "\nNo flights available from source to Destination, you can check back Later!!";
			return false;
		}
	}
	
	return true;
}

// used to check seat availablity
void checkAvailableSeats(string id)
{
	int indexOfGivenId = getIndexOfFlightId(id);
	
	printBusinessClassArrangement(indexOfGivenId);
   	printEconomyClassArrangement(indexOfGivenId);
  
}

// function to book flight
void bookFlight()
{
	string id, seatNumber;
	char ch, bOre;
    int noOfSeats,totalAmount = 0;
	
	// first search the flight if available or not
	if(!searchFlight()) return;
	
	cout << "\nEnter Flight Id to check for seat allocation:";
	cin >> id;
	
	// fetch the available seats from available seats folder
	cout << "\nSeat Availablity\n";
	checkAvailableSeats(id);
	printSeatInfo();
	
	// get seat number
	cout << "\nEnter the number of Tickets : ";
	cin >> noOfSeats;	
	
	for(int i=0; i<noOfSeats; i++)
	{
		cout << "\n\n Press (B/E) to Book Business/Economy class for " << i+1 << " ticktet ";
	    cin >> bOre;
	    
		cout << "\n Enter the available Seat number [Seat number will be Row_column (For eg: 5_F represents 5th row, 6th Column)] ";
	    cin >> seatNumber;
	
    	cout << "\n Are u sure want to book this ticket (Press Y/N)? "; 
		cin >> ch;
				
		if(ch == 'Y')
		{
	        updateSeatInCorrespondingFlight(id, seatNumber, bOre, totalAmount);
		}
		else if(ch == 'N')
		{
			i--;
		}
		else {
			cout << "\nInvalid option";
			i--;
		}
		
	}
	
	cout << endl;
	cout << noOfSeats << " Tickets Booked Successfully\n";
	cout << "\nTotal Price of tickets : \n" << totalAmount;
	
}

// function to reset all information
bool resetSeat(string id, char businessOrEconomy, int bookingId)
{
	int indexOfGivenId = getIndexOfFlightId(id);
	if(indexOfGivenId == -1)
	{
		cout << "\nInvalid Id";
		return false;
	}
	int indexOfPassenger = bookIdRef[bookingId];
	
	string seatNumber = passObj[indexOfPassenger].seatNum;
	
	int rowNum = -1, colNum = -1;
	convertToSeatNumber(rowNum, colNum, seatNumber);
	
	// removing references
	bookIdRef.erase(bookingId);
	passObj.erase(passObj.begin()+indexOfPassenger);
	totalPassCount--;
	
	// if its business class
	if(businessOrEconomy == 'b'|| businessOrEconomy == 'B')
	{
		// resetting all values
		businessRow[indexOfGivenId][rowNum][colNum] = businessRowCopy[indexOfGivenId][rowNum][colNum];
		writeAndGetFileInfo(indexOfGivenId);
	}
	
	else if(businessOrEconomy == 'e'|| businessOrEconomy == 'E')
	{		
		// resetting all values
		economyRow[indexOfGivenId][rowNum][colNum] = economyRowCopy[indexOfGivenId][rowNum][colNum];
    	writeAndGetFileInfo(indexOfGivenId);
	}
	
	return true;
}

// function to handle cancellation process
void cancelFlight()
{
	if(passObj.size() == 0)
	{
		cout << "\n No Seats Alloted Previously";
		return;
	}
	
	int numSeats, cancellationFees = 0;
	
	cout << "\nEnter number of Seats to cancel ";
	cin >> numSeats;
	for(int i=0; i<numSeats; i++)
	{
	string id;
	cout << "\n Enter Flight Id from you are going to cancel Ticket : ";
	cin >> id;
		
	int bookingId;
	cout << "\n Enter your booking Id : ";
	cin >> bookingId;
	
	// getting which class
	char businessOrEconomy = passObj[bookIdRef[bookingId]].busOrEcon;
	
	char option;
	do{
		cout << "\n Are you sure want to cancel this ticket (Y/N) ? ";
		cin >> option;	
	}while(option != 'Y');
	
	if(option == 'Y')
	{
		if(!resetSeat(id, businessOrEconomy, bookingId)){
			cout << "\nCannot cancel at a moment, Try again later !!\n";
		}	
		else
		cancellationFees += 200;	
	}
	else
	{
		i--;
	}
	
	}
	
	cout << "\n Tickets Cancelled Successfully";
	cout << "\n Your Cancellation Fees : " << cancellationFees;
	
}

// function to print booking summary of all passengers
void bookSummary()
{
	if(totalPassCount > 0)
	{
	cout << "\n Booking Details : ";
	cout << "\n FlightId  BookingId  Passenger_Name  Passenger_Age  SeatNumber MealOrder  BusinessOrEconomy  TicketPrice\n";
	for(int i=0; i<totalPassCount; i++)
	{
		cout << setw(8) << passObj[i].passFlightId << setw(10) << passObj[i].bookingId << setw(12) << passObj[i].names << setw(15) << passObj[i].ages << setw(15) << passObj[i].seatNum << setw(8);
		cout << passObj[i].isMealOrdered << setw(15) << passObj[i].busOrEcon << setw(18) << passObj[i].ticketPrice << endl;
	}
	}
	else{
		cout << "\n No Data Available. Kindly please add some content";
		return;
	}

}

// function to get seat number for each flight Id
void getMealOrderedSeatNumber()
{	
	cout << "\nSeat Numbers Of Meal Ordered : ";
	for(int i=0; i<flightObj.listCount; i++)
	{
		string id = flightObj.flightId[i];
		int flag = 0;
		cout << "\n\nFlight Id : "  << id << endl;

		for(int j=0; j<totalPassCount; j++)
		{
			if(passObj[j].isMealOrdered == 'Y' && passObj[j].passFlightId == id)
			flag = 1, cout <<  "Seat Number : " << passObj[j].seatNum ;
		}
	
		if(!flag) cout << "No meals ordered for this flight\n";
	}
}

int main()
{
	getAllFilesInsideFolder();	
	fetchAndWriteFileContents();
	
	int option;
	char ch;
	string id;
	cout << "Welcome to Flight Ticket Booking System!!";
	do
	{	
	cout << "\nList of Functions you can perform : ";
	cout << "\n1) List the number of Flights and name of the flights";
	cout << "\n2) Search Flights";
	cout << "\n3) Book Flights";
	cout << "\n4) Cancel Flights";
	cout << "\n5) Booking Summary";
	cout << "\n6) Available Seats";
	cout << "\n7) Get meal Ordered Seat per Flight";
	cout << "\n Enter any number from 1 to 7 to perform corresponding function ";
	cin >> option;
	
	switch(option)
	{
		case 1 : listFlight(); break;
		
		case 2 : searchFlight(); break;
		
		case 3 : bookFlight(); break;
		
		case 4 : cancelFlight(); break;
		
		case 5 : bookSummary(); break;
		
		case 6 : cout << "Enter Flight id to Search: "; 
		         cin >> id; 
		         checkAvailableSeats(id); 
				 break;
		
		case 7 : getMealOrderedSeatNumber();
				 break;
				 
		default : cout << "Invalid Number";
	}
	
	cout << "\nPress Y to continue...";
	cin >> ch;
	
    }while(ch == 'Y');

	
	
	return 0;
}
