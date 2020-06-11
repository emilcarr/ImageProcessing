//    This program is free software: you can redistribute it and/or modify
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

#include<CImg.h>

namespace po = boost::program_options;
namespace cimg = cimg_library;
using namespace std;

typedef struct pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

int perceivedBrightness(pixel p){
    return (int)0.2126*p.r + 0.7152*p.g + 0.0722*p.b;
}

char asciiMap[256] = "     ~~~~~~~~::::::::::``````++++++iiiiii'''''///||((rxxll<<<IIII{{{!!ccoo[[ttppn=gggkkk""11144^^^^^^^fffffffAAAAddbhh332227UUVV???55000&&F966CCEE88888$$$$$%%%#######OOOOOBBBBPNNNNNRRRRRRRRRRRRRRRRMMMMMMMMMMMMMMMMMMMMWWWWWWWWWWWWWWWWWWWWWW@@@@@@@@@@@@@@@@";

int main(int argc, char **argv) {
    stringstream dstr;
    dstr << "USAGE: " << argv[0] << " [, options] infile" << "\n\n"
        << "Convert an image to ASCII art.";

    po::options_description desc(dstr.str());
    po::positional_options_description pos;
    
    desc.add_options()
        ("help,h", "Display this help message and exit.")
        ("infile, i", po::value<string>()->default_value("in.jpg"), "The image to convert to ASCII.")
        ("downscaling,s", po::value<float>()->default_value(4), "The downscaling factor.")
    ;

    pos.add("infile", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
 
    cimg::CImg<unsigned char> image(vm["infile"].as<string>().c_str());
    
    float ds = vm["downscaling"].as<float>();
    image.resize(image.width() / ds, image.height() / ds, image.depth(), image.spectrum(), 3, 0, 0, 0, 0, 0);

    int width = image.width();
    int height = image.height();
    int depth = image.depth();
    
    image.permute_axes("cxyz"); // Convert buffer into interleaved format (RGBRGBRGB...)
    pixel* iptr = (pixel*)image.data();

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++)
            cout << asciiMap[perceivedBrightness(iptr[y*width*depth + x])];
        cout << endl;
    }
}
