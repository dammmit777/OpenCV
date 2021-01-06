#include <opencv2/core.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

static void help(char **argv)
{
    cout << endl
         << argv[0] << " Shows the usage of the OpenCV serialization functionally." << endl
         << "Usage: " << endl
         << argv[0] << " outputfile.yml.gz" << endl
         << "The output file may be either XML or YAML.You can even compress it by"
         << "specifying this in its extension like xml.gz yaml.gz etc..." << endl
         << "With FileStorage you can serialize objects in OpenCV by using the << and >> operators" << endl
         << "For example: -create a class and have it serialized" << endl
         << "-use it to read and write matrices." << endl;
}

class MyData
{
public:
    MyData() : A(0), X(0), id() {}
    explicit MyData(int) : A(97), X(CV_PI), id("mydata1234") {} //explicit to avoid implicit conversion

    //![inside]
    //Write serialization for this class
    void write(FileStorage &fs) const
    {
        fs << "{"
           << "A" << A << "X" << X << "id" << id << "}";
    }

    //Read serialization for this class
    void read(const FileNode &node)
    {
        A = (int)node["A"];
        X = (double)node["X"];
        id = (string)node["id"];
    }
    //![inside]

public: //Data Members
    int A;
    double X;
    string id;
};

//These write and read functions must be defined for the serialization in FileStorage to work
//![outside]
static void write(FileStorage &fs, const std::string &, const MyData &x)
{
    x.write(fs);
}

static void read(const FileNode &node, MyData &x, const MyData &default_value = MyData())
{
    if (node.empty())
        x = default_value;
    else
        x.read(node);
}
//![outside]

//This function will print our custom class to the console
static ostream &operator<<(ostream &out, const MyData &m)
{
    out << "{ id= " << m.id << ", ";
    out << "X = " << m.X << ", ";
    out << "A =" << m.A << "}";
    return out;
}

/**
 * @brief Main
 *
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        help(argv);
        return 1;
    }

    string filename = argv[1];

    {
        //write
        //![iomat]
        Mat R = Mat_<uchar>::eye(3, 3),
            T = Mat_<double>::zeros(3, 1);
        //![iomat]

        //![customIOi]
        MyData m(1);
        //![customIOi]

        //![open]
        FileStorage fs(filename, FileStorage::WRITE);
        /*or:
         * FileStorage fs;
         * fs.open(filename,FileStorage::WRITE);
         */
        //![open]

        //![writeNum]
        fs << "iterationNr" << 100;
        //![writeNum]

        //![writeStr]
        fs << "strings"
           << "[";
        fs << "image1.jpg"
           << "Awesomeness"
           << "IO_image/2.jpeg";
        fs << "]";
        //![writeStr]

        //![writeMap]
        fs << "Mapping";
        fs << "{"
           << "One" << 1;
        fs << "Two" << 2 << "}";
        //![writeMap]

        //![iomatw]
        fs << "R" << R;
        fs << "T" << T;
        //![iomatw]

        //![custonIOw]
        fs << "MyData" << m;
        //![custonIOw]

        //![close]
        fs.release();
        //![close]
        cout << "Write Done." << endl;
    }

    {
        //read
        cout << endl
             << "Reading: " << endl;
        FileStorage fs;
        fs.open(filename, FileStorage::READ);

        //![readNum]
        int itNr;
        //fs["iterationNr"]>>itNr;
        itNr = (int)fs["iterationNr"];
        //![readNum]
        cout << itNr;

        if (!fs.isOpened())
        {
            cerr << "Failed to open " << filename << endl;
            help(argv);
            return 1;
        }

        //![readStr]
        FileNode n = fs["strings"];
        if (n.type() != FileNode::SEQ)
        {
            cerr << "Strings is not a sequence! FAIL" << endl;
            return 1;
        }

        FileNodeIterator it = n.begin(), it_end = n.end();
        for (; it != it_end; ++it)
            cout << (string)*it << endl;
        //![readStr]

        //![readMap]
        n = fs["Mapping"];
        cout << "Two" << (int)(n["Two"]) << ";";
        cout << "One" << (int)(n["One"]) << endl
             << endl;
        //![readMap]

        MyData m;
        Mat R, T;

        //![iomat]
        fs["R"] >> R;
        fs["T"] >> T;
        //![iomat]

        //![customIO]
        fs["MyData"] >> m;
        //![customIO]

        cout << endl
             << "R= " << R << endl;
        cout << "T= " << T << endl
             << endl;
        cout << "MyData= " << endl
             << m << endl
             << endl;

        //Show default behabior for non existing nodes
        //![nonexist]
        cout << "Attempt to read NonExisting(shoule initialize the data structure with its default).";
        fs["NonExisting"] >> m;
        cout << endl
             << "NonExisting= " << endl
             << m << endl;
        //![nonexist]
    }

    cout << endl
         << "Tip:Open up " << filename << " with a text editor to see the derialized data." << endl;

    return 0;
}
