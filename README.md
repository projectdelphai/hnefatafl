hnefetafl
=================
Reuben Castelino - projectdelphai@gmail.com

Description
----------------
A viking game similar to chess. I was inspired to create a game based on [this post on Medium](https://medium.com/war-is-boring/cef088ae4e2d) and I have followed [this site](http://tafl.cyningstan.org.uk/page/21/the-hnefatafl-board) for most of the practical and technical rules

I am coding this as an introductory lesson into c++. So there may be problems in how I make it in regards to efficiency and proper standards. Correct me if I make any errors.

Installation
----------------
Currently, there is only verified support for linux platforms (specifically archlinux). 

Dependencies:

 1. gcc/g++

Packaged Dependencies:

 1. json-cpp

To compile the package:

    g++ -L../hnefatafl -ljson_linux-gcc-4.8.2_libmt core.cpp

Then run it with:

    ./a.out

Contributing
---------------

 1. Fork the source
 1. Create a branch
 1. Make your changes
 1. Push to your branch
 1. Make a pull request
