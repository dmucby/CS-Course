#include <iostream>
#include <sstream>
#include <typeinfo>

using std::cout;    using std::endl;
using std::cin;     using std::string;


string IntegerToString(int myInt) {
    /*
     * We'll specifically use an ostringstream, which is just a
     * stringstream that you can only put things into.
     */
    std::ostringstream conventer;
    /*
     * Putting the int into the ostringstream will convert
     * it into a string format.
     */

    conventer << myInt;

    /* Ask for the string inside the ostringstream. */
    return conventer.str();
}

int StringToInteger(const string& str) {
    /*
     * We'll specifically use an istringstream, which is just a
     * stringstream that you can only get things from.
     *
     * You can set its internal string when creating it or by doing
     * converter.str("string_to_set");
     */
    std::istringstream in(str);

    /*
     * Try getting an int from the stream. If this is not succesful
     * then user input was not a valid input.
     */
    int value;
    if(in >> value){

        /*
         * See if we can extract a char from the stream.
         * If so, the user had junk after a valid int in their input.
         */
        char res;

        if(in >> res){
            /*
             * Throwing an error is a way of propogating up the funcion
             * callstack that something went wrong. Previous functions can
             * anticipate such an error and catch it. In this case, we will
             * just let the thrown domain_error propogate up until it terminates
             * the program.
             *
             * A domain_error is defined in the standard namespace as an error
             * to signal that the input arguments to the function were not valid.
             */
            throw std::domain_error(string("Unexpected character in input: ") + res);
        }

        return value;
    }
    /* If read unsuccessful, throw a domain error with a helpful error message. */
    throw std::domain_error(string("Failed to convert input: ") + str);
}

int getInteger() {
    while(true) {
        /* First we get a line of input from the user. */
        string line;
        std::getline(cin, line);
        /*
         * We'll again use an istringstream because we only want to pull data
         * out of the stream once we have put the string's contents in it.
         */
        std::istringstream converter(line);
        /*
         * Try getting an int from the stream. If this is not succesful
         * then user input was not a valid input.
         */
        int result;
        if(converter >> result) {
            /*
             * See if we can extract a char from the stream.
             * If so, the user had junk after a valid int in their input.
             */
            char remaining;
            if(converter >> remaining) {
                cout << "Unexpected character. Try again." << endl;
            } else {
                /*
                 * Input was succesfully converted to int with no
                 * trailing stuff at the end.
                 */
                return result;
            }
        } else {
            cout << "Not a valid integer. Try again." << endl;
        }
    }
}

int main() {
   int x = getInteger();
   cout << "We got: " << x << endl;
   cout << "the IntToString:" << IntegerToString(x) << endl;
   cout << "str ?????????" << typeid(x).name() << endl;

   cout << "==========================" << endl;

//    string str;
//    getline(cin,str);
//    cout << "We got the string: " << str << endl;
//    cout << "str ?????????" << typeid(str).name() << endl;

//    cout << "the StringToInt:" << StringToInteger(str) << endl;
//    cout << "str ?????????" << typeid(str).name() << endl;
}
