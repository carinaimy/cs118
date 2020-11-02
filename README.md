# cs118_fall2020_project1

1. Hign-level design of server:
    When the server runs, it first checks the correctness of the parameters, 
  especially the port number. If the port number is wrong, the server prints the error message to
  stderr. Otherwise, the server opens the socket and listens for user connections at this port
  number. Before listening for client connection requests, the server initializes CRC table using
  interface create_crc_table(), and records connection ID with variable connectionId. When receiving
  the connection request from the client, the server creates a new thread to complete the data
  transmission. Because of threads, the server can process requests from multiple clients at the
  same time. The server passes the ID and the socket of a client connection to the thread handleReq().
  In handleReq, a out file is created. Segments will be received, checked, and output to the out file
  in a loop. Timeout error will be handled in the loop.
  
  Hign-level design of client:
    As with the server, the client also checks the correctness of the parameters and initializes the
  CRC table. In addition, the client checks the file size and creates a connection to the server.
  The client will read the file circularly, calculate the CRC, fill the CRC into segments, and send
  it to the server.
  
2. problems and solution
    (1)The output file creation failed. I experienced that the outfile doesn't write to the save folder,
    after setting the path as a relative path, the problem has been solved.
    (2)Also, therecv was blocking during the process, therefore, i used the setsockopt to set the timeout.
    
3. additional libraries
     only pthread library is used in my codes.