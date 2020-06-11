//     This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h> // srand

#include <CImg.h>

namespace po = boost::program_options;
namespace cimg = cimg_library;
using namespace std;

typedef struct pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

float perceivedBrightness(pixel p){
    return 0.2126*p.r + 0.7152*p.g + 0.0722*p.b;
}

template <typename T>
T* bubbleSort(int, T*, int = 0, int = NULL, int = 1, double = 0.0);
template <typename T>
T* bubbleSort(int n, T* toSort, int threshold, int maxDepth, int increment, double pswap) {
    int depth = 0;
    while(depth != maxDepth){
        bool swapped = false;
        T *loc = toSort + increment;
        for(loc; loc != toSort+(n*increment); loc += increment){
            if(perceivedBrightness(*(loc-increment)) - perceivedBrightness(*loc) > threshold){
		if(rand() > pswap/100*RAND_MAX){
		    T high = *(loc-increment);
		    *(loc-increment) = *(loc);
		    *(loc) = high;
		}
                swapped = true;
            }
        }
        if(!swapped)
            break;
        depth++;
    }

    return toSort;
}

int main(int argc, char **argv){
    
    srand(time(NULL)); // initialise pseudo random number seed

    stringstream dstr;
    dstr << "USAGE: " << argv[0] << " [, options] infile outfile" << "\n\n"
        << "Pixel sort an image with various options";

    po::options_description desc(dstr.str());
    po::positional_options_description pos;

    desc.add_options()
        ("help,h", "Display this help message and exit.")
        ("infile,i", po::value<string>()->default_value("in.jpg"), "The input file for processing.")
        ("outfile,o", po::value<string>()->default_value("out.jpg"), "The file in which to store the processed output.")
        ("depth,d", po::value<int>()->default_value(50), "The maximum number of sorting passes.")
        ("threshold,t", po::value<int>()->default_value(0), "The threshold for pixel swapping.")
        ("axis,a", po::value<string>()->default_value("x"), "The axis along which to sort (x,y,-x,-y,l,-l) (chainable).")
	("pswap,ps", po::value<double>()->default_value(0.0), "% probability to ignore pixel swap.")
	("paxis,pa", po::value<int>()->default_value(0), "% probability to skip one row/column.")
    ;
    
    pos.add("infile", 1);
    pos.add("outfile", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    cimg::CImg<unsigned char> image(vm["infile"].as<string>().c_str());

    int width = image.width();
    int height = image.height();
    int depth = image.depth();
    
    image.permute_axes("cxyz"); // Convert buffer into interleaved format (RGBRGBRGB...)
    
    char *ax = (char*)vm["axis"].as<string>().c_str();

    while(*ax != '\0'){
	
        bool neg = false;
        if(*ax == '-') {
            neg = true;
            ax++;
            if(*ax == '\0')
                break;
        }

        switch(*ax){

            case 'l':
                if(neg)
                    bubbleSort<pixel>(image.size()/sizeof(pixel), (pixel*)image.data() + width*height*depth, vm["threshold"].as<int>(), vm["depth"].as<int>(), -1, vm["pswap"].as<double>());
                else
                    bubbleSort<pixel>(image.size()/sizeof(pixel), (pixel*)image.data(), vm["threshold"].as<int>(), vm["depth"].as<int>(), 1, vm["pswap"].as<double>());
                break;

            case 'x':
                if(neg)
                    for(int y = 0; y < height; y++) {
	    		if(rand() % 100 < vm["paxis"].as<int>()) continue;
                        bubbleSort<pixel>(width*depth, (pixel*)image.data() + y*width*depth + width, vm["threshold"].as<int>(), vm["depth"].as<int>(), -1, vm["pswap"].as<double>());
		    }
                else
                    for(int y = 0; y < height; y++) {
	    		if(rand() % 100 < vm["paxis"].as<int>()) continue;
                        bubbleSort<pixel>(width*depth, (pixel*)image.data() + y*width*depth, vm["threshold"].as<int>(), vm["depth"].as<int>(), 1, vm["pswap"].as<double>());
		    }
                break;

            case 'y':
                if(neg)
                    for(int x = 0; x < width; x++) {
	    		if(rand() % 100 < vm["paxis"].as<int>()) continue;
                        bubbleSort<pixel>(height*depth, (pixel*)image.data() + x + width*depth*height, vm["threshold"].as<int>(), vm["depth"].as<int>(), 0-width*depth, vm["pswap"].as<double>());
		    }
                else
                    for(int x = 0; x < width; x++) {
	    		if(rand() % 100 < vm["paxis"].as<int>()) continue;
                        bubbleSort<pixel>(height*depth, (pixel*)image.data() + x, vm["threshold"].as<int>(), vm["depth"].as<int>(), width*depth, vm["pswap"].as<double>());
		    }
                break;

            default:
                cout << "Unrecognised axis \"" << *ax << "\"." << endl;
                return 1;

        }
        ax++;
    }
    
    image.permute_axes("yzcx"); // Convert back into original format (RRR...GGG...BBB...)

    image.save(vm["outfile"].as<string>().c_str());

    //cimg::CImgDisplay disp(image, vm["outfile"].as<string>().c_str());
    //while(!disp.is_closed()){}

    return 0;
}
