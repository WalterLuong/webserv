#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

// DEFINITIONS //

#define		PORT 4000		// Define the port to connect to
#define		MAX_CLIENTS 10	// Define the maximum number of clients we can receive
#define		BUFFER_SIZE 256 // Define the buffer size of the messages

// STRUCTURES //

struct _client
{
	bool		connected;
	sockaddr_in	address;
	SOCKET		socket;
	fd_set		socket_data;
	int			address_length;

	char		template_name[15];
	char		screen_name[15];
	char		siegepos[45];
};

// GLOBAL VARIABLES //

sockaddr_in	server_address;
sockaddr	server_socket_address;
SOCKET		server_socket;

_client		client[MAX_CLIENTS];
int			clients_connected = 0;

// FUNCTION DECLARATIONS //

bool	accept_client ( _client *current_client );
int		accept_connections ();
int		disconnect_client ( _client *current_client );
void	echo_message ( char *message );
void	end_server();
void	midcopy ( char* input, char* output, int start_pos, int stop_pos );
int		receive_client ( _client *current_client, char *buffer, int size );
void	receive_data();
int		send_data ( _client *current_client, char *buffer, int size );
void	start_server();

// FUNCTION DEFINITIONS //

bool accept_client ( _client *current_client )
{
	// Accept incoming connections
	current_client->address_length = sizeof ( sockaddr );
	current_client->socket = accept ( server_socket, ( sockaddr * ) &current_client->address, &current_client->address_length );

	if ( current_client->socket == 0 )
	{
		// No data in socket
		return ( FALSE );
	}
	else if ( current_client->socket == SOCKET_ERROR )
	{
		// Socket error
		return ( FALSE );
	}
	else
	{
		// Occupy the client slot
		current_client->connected = TRUE;
		FD_ZERO ( &current_client->socket_data );
		FD_SET ( current_client->socket, &current_client->socket_data );

		return ( TRUE );
	}

	return ( FALSE );
}

int accept_connections()
{
	if ( clients_connected < MAX_CLIENTS )
	{
		for ( int j = 0; j < MAX_CLIENTS; j++ )
		{
			if ( !client[j].connected )
			{
				if ( accept_client ( &client[j] ) )
				{
					// Increment the client count
					clients_connected++;

					// Grab the ip address of the client ... just for fun
					char *client_ip_address = inet_ntoa ( client[j].address.sin_addr );

					// Output connection
					cout << "ACCEPTING CLIENT to array position [" << j << "] with IP ADDRESS " << client_ip_address << endl;
				}
			}
		}
	}

	return ( 1 );
}

int disconnect_client ( _client *current_client )
{ // Disconnect a client
	if ( current_client->connected == TRUE )
	{ // Close the socket for the client
		closesocket ( current_client->socket );
	}

	// Set the new client state
	current_client->connected = FALSE;

	// Clear the address length
	current_client->address_length = -1;

	// Decrement the current number of connected clients
	clients_connected--;

	// Declare a variable to store the disconnect message into
	char raw_data[BUFFER_SIZE];

	// Parse in the client data to send
	sprintf ( raw_data, "~4 %s", current_client->screen_name );

	// Echo out the disconnect message so all clients drop this client
	echo_message ( raw_data );

	cout << "Disconnecting client[]" << endl;

	return ( 1 );
}

void echo_message ( char *message )
{
	for ( int j = 0; j < MAX_CLIENTS; j++ )
	{
		if ( client[j].connected )
		{ // Echo the message to all clients
			send_data ( &client[j], message, BUFFER_SIZE );
		}
	}
}

void end_server()
{ // Shut down the server by disconnecting all clients and clearing winsock
	// Disconnect all clients
	for ( int j = 0; j < MAX_CLIENTS, j++;) { disconnect_client ( &client[j] ); }

	// Close the listening socket for the server
	closesocket ( server_socket );

	// Clean up winsock
	WSACleanup();
}

void midcopy ( char* input, char* output, int start_pos, int stop_pos )
{
	int index = 0;

	for ( int i = start_pos; i < stop_pos; i++ )
	{
		output[index] = input[i];
		index++;
	}

	output[index] = 0;
}

int receive_client ( _client *current_client, char *buffer, int size )
{
	if ( FD_ISSET ( current_client->socket, &current_client->socket_data ) )
	{
		// Store the return data of what we have sent
		current_client->address_length = recv ( current_client->socket, buffer, size, 0 );

		if ( current_client->address_length == 0 )
		{ // Data error on client
			disconnect_client ( current_client );

			return ( FALSE );
		}

		return ( TRUE );
	}

	return ( FALSE );
}

void receive_data()
{
	char buffer[BUFFER_SIZE];

	for ( int j = 0; j < MAX_CLIENTS; j++ )
	{
		if ( client[j].connected )
		{
			if ( receive_client ( &client[j], buffer, BUFFER_SIZE ) )
			{
				if ( buffer[0] == '~' )
				{ // All data should be buffered by a '~' just because

					if ( buffer[1] == '1' ) // Add Client Command
					{
						// Declare the buffer to store new client information into
						char raw_data[BUFFER_SIZE];

						// Parse out the 'Add Client' command
						midcopy ( buffer, raw_data, 3, strlen ( buffer ) );

						// Store the client information into our RAM client database
						sscanf ( raw_data, "%s %s %s", client[j].template_name, client[j].screen_name, client[j].siegepos );

						for ( int k = 0; k < MAX_CLIENTS; k++ )
						{
							if ( ( client[k].connected ) && ( j != k ) )
							{
								// Parse in the client data to send
								sprintf ( raw_data, "~1 %s %s %s", client[k].template_name, client[k].screen_name, client[k].siegepos );

								// Send the client data
								send_data ( &client[j], raw_data, BUFFER_SIZE );
							}
						}
					}
					else if ( buffer[1] == '2' ) // Move Client Command
					{
						// Declare the buffer to store new client information into
						char raw_data[BUFFER_SIZE];

						// Parse out the 'Move Client' command
						midcopy ( buffer, raw_data, 3, strlen ( buffer ) );

						// Update the client information into our RAM client database
						sscanf ( raw_data, "%s %s", client[j].screen_name, client[j].siegepos );
					}
					else if ( buffer[1] == '3' ) // Chat Client Command
					{
						// ECHO THE MESSAGE BACK TO ALL CLIENTS
					}
					else if ( buffer[1] == '4' ) // Remove Client Command
					{
						// Disconnect the current client
						disconnect_client ( &client[j] );
					}

					// Display all data received
					// cout << buffer << endl;

					// Echo the message to the other clients
					echo_message ( buffer );

					// Clear the buffer
					buffer[0] = '/0';
				}
			}
		}
	}
}

int send_data ( _client *current_client, char *buffer, int size )
{
	// Store the return information about the sending
	current_client->address_length = send ( current_client->socket, buffer, size, 0 );

	if ( ( current_client->address_length == SOCKET_ERROR ) || ( current_client->address_length == 0 ) )
	{ // Check for errors while sending
		disconnect_client ( current_client );

		return ( FALSE );
	}
	else return ( TRUE );
}

void start_server()
{ // Initialize the server and start listening for clients
	// LOCAL VARIABLES //
	int res, i = 1;

	// Set up the address structure
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons ( PORT);

	// IM GUESSING : Copy over some addresses, conversions of some sort ?
	memcpy ( &server_socket_address, &server_address, sizeof ( SOCKADDR_IN ) );

	// Create a listening socket for the server
	server_socket = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( server_socket == INVALID_SOCKET )
	{
		cout << "SOCKET ERROR : Invalid socket." << endl;
	}
	else if ( server_socket == SOCKET_ERROR )
	{
		cout << "SOCKET ERROR : Socket error." << endl;
	}
	else
	{
		cout << "SOCKET ESTABLISHED" << endl;
	}

	// Sets the option to re-use the address the entire run of the program
	setsockopt ( server_socket, SOL_SOCKET, SO_REUSEADDR, ( char * ) &i, sizeof ( i ) );

	// Bind the socket to the address
	res = bind ( server_socket, &server_socket_address, sizeof ( server_socket_address ) );

	cout << "Binding socket:" << res << endl;

	if ( res != 0 )
	{
		cout << "BINDING ERROR : Failed to bind socket to address." << endl;
	}
	else
	{
		cout << "Socket Bound to port : "<< PORT << endl;
	}

	// Start listening for connection requests
	res = listen ( server_socket, 8 );

	// This makes the server non blocking, hence it won't wait for a response
	unsigned long b = 1;
	ioctlsocket ( server_socket, FIONBIO, &b );

	// Clear all clients state
	for ( int j = 0; j < MAX_CLIENTS; j++ ) { client[j].connected = FALSE; }
}

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

// MAIN FUNCTION //

int main()
{
	cout << "\txmen90s Non-Blocking Multi-Client Echo Server for Dungeon Siege\n" << endl;

	// Initialize winsock and start listening
	start_server();

	// Loop forever
	bool looping = TRUE;

	while ( looping )
	{
		// Accept all incoming client connections
		accept_connections();

		// Receive all data from clients
		receive_data();
	}

	// Shut down winsock
	end_server();

	return 0;
}