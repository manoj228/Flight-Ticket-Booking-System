# Flight-Ticket-Booking-System using C++
Sample Flight Ticket Booking System

# Features of the application :

* Every inforamtion in the files , which user given are automatically fetched (offline) <br/>
* Application is implemented using C++ <br/>
* All the fetched information will be displayed initially for easy analyze <br/>
* User can get all the available flights and its count <br/>
* User can search flights by giving source and destination as input and will get all the available flights <br/>
* User can book flights by giving corresponding inputs <br/>
* System has more interactive with the user <br/>
* User can cancel flight by giving booking id as input <br/>
* User will also get available seats using giving flight id and booking summary <br/>

# Purpose of each Functions:

* listFlight() : 
  - Helps user to get list of all flights names and the counts of flights.

* searchFlight() :
  - Helps user to search if flights available from source to destination.
 
* bookFlight() :
  - Helps user to book flights by giving user information and based on availablity of each flight information.
 
* cancelFlight() :
  - Helps user to cancel flight tickets by using booking id.

* bookSummary() :
  - Function used to display the users who booked current session.

* checkAvailableSeats() :
  - Helps user to check all the available seats based on user given flight Id.

* getMealOrderedSeatNumber() : 
  - Function used to get the seat number of meal order passenger based on flight wise.
