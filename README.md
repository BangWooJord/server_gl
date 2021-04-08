# Server - client relation

TCP asynchronous server and client. After server have been started and client succesfully connected to it, client can send messages that will be recieved (and shown) by server.
Both server and client have correct input verification and all kinds of service messages, that let user know what's hapenning.

Technologies: 
  - C++ (17)
  - Boost (1.75.0) - https://www.boost.org - mainly boost::asio for networking. And some other libraries that help. 

# Note
  I didn't add Boost as a submodule, since you need to build it by hand, in order to work with it. And it's quite heavy. 
