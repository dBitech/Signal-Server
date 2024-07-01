**Note: This repo was forked from the CloudRF Signal-Server repo before that repository was deleted sometime in late 2023. Any updates and changes since then will be documented appropriately.**

# Signal Server
Multi-threaded radio propagation simulator based upon SPLAT! by Alex Farrant QCVS, 2E0TDW. 

SPLAT! Project started in 1997 by John A. Magliacane, KD2BD

Some additional features and fixes by Aaron A. Collins, N9OZB
                                      Tom Hayward, KD7LXL
                                      Darcy Buskermolen, VA7DBI

This server application will generate RF coverage predictions, producing either 2D profile plots (Point-to-Point) or 360 degree polar plots in WGS-84 projection as PPM Bitmaps. 

For detailed information and historical reference data related to this project see the SPLAT! documentation. This SPLAT! fork used to power CloudRF.com from 2012 to 2016 before it was replaced with a purpose built engine. Propagation models added to this project have been sourced from reputable academic sources and all efforts have been taken to ensure their accurate implementation. Not all models are ITU ratified and you use them entirely at your own risk.

WARNING: The accuracy of the output is directly proportional to the accuracy of the inputs and the time taken defining and validating them. 


## Requirements
* C++14-conformant C++ compiler (GCC,G++ / clang)
* Build environment for C++ (linker, C++ Standard Library and so forth) 
* CMake v3.5 or newer
* Convert (part of ImageMagick)
* For some additional scripts: Bash and Python interpreter
* library pthread: POSIX threads library
* library bz2: The bzip2 runtime library
* library dl: Open and close a shared object - POSIX conform
* library z: zlib is a general-purpose lossless data-compression library
* Multicore CPU (optional but recomended)
* ~2GB Memory
* SRTM terrain tile(s) or ASCII Grid tile(s)

Signal Server is a very resource intensive multicore application. Only publish it for common use if you know what you are doing and you are advised to wrap it with another script to perform input validation.

Additional programs/scripts will be required to prepare inputs such as .hgt tiles (srtm2sdf.c), 3D antenna patterns (.ant) and user defined clutter (.udt) or manipulate the bitmap output (.ppm). More information can be found in the SPLAT! project.

## File extensions and types used by signalserver:
```
.asc LIDAR topo data file in ASCII grid format
.jpg LIDAR topo data file in JPEG format
.sdf SPLAT! topo data file in SPLAT! format, lo-res 90m (from SRTM3)
.sdf.gz topo data file in SPLAT! data format, gzip compressed, lo-res 90m
.sdf.bz2 topo data file in SPLAT! data format, bzip2 compressed, lo-res 90m
-hd.sdf SPLAT! topo data file in SPLAT! format, hi-res 30m (from SRTM1)
-hd.sdf.gz topo data file in SPLAT! data format, gzip compressed, hi-res 30m
-hd.sdf.bz2 topo data file in SPLAT! data format, bzip2 compressed, lo-res 90m
.scf signal level color palette file
.lcf loss level color palette file
.dcf dbm level color palette file
.az SPLAT! antenna pattern azimuth data file
.el SPLAT! antenna pattern elevation data file
.lrp LIDAR antenna pattern data file
.udt user defined terrain data clutter data text file
.sh  miscellaneous shell scripts and batch files
.ppm portable pixmap - output plot graphic rendering (native)
.png portable network graphics - output plot graphic rendering (converted)
.kml Google Earth Keyhole Markup Language - output viewable with Google Earth
.kmz Google Earth Keyhole Markup Language, compressed
```

## Install dependencies
Assuming Debian / Ubuntu, this will fetch the core libraries needed to build it as well as an image library for manipulating outputs.
```
sudo apt-get install g++ cmake libbz2-dev imagemagick spdlog
```

## Installation
Change into the source directory to build the binaries.
```
mkdir build
cd build
cmake ../src
make
```

## Test (needs updating)
Run the test script from the top level directory. Binaries are in the src directory.
Test output will be in output/tests
```
./test.sh
```

## Parameters
```
----------------------------------------------------------------------------------
Signal Server 4.0 (master c1e8c72)
    Compile date: Jul  1 2024 01:05:46
    Built for 32 DEM tiles at 3600 pixels
----------------------------------------------------------------------------------
License: GNU General Public License (GPL) version 2

Radio propagation simulator by Alex Farrant QCVS, 2E0TDW
Based upon SPLAT! by John Magliacane, KD2BD
Some feature enhancements/additions by Aaron A. Collins, N9OZB
Additional improvements and multithreading fixes by P. McDonnell, W3AXL

Usage: signalserver [data options] [input options] [antenna options] [output options] -o outputfile

Data:
     -sdf Directory containing SRTM derived .sdf DEM tiles (may be .gz or .bz2)
     -lid ASCII grid tile (LIDAR) with dimensions and resolution defined in header
     -udt User defined point clutter as decimal co-ordinates: 'latitude,longitude,height'
     -clt MODIS 17-class wide area clutter in ASCII grid format
     -color File to pre-load .scf/.lcf/.dcf for Signal/Loss/dBm color palette
Input:
     -lat Tx Latitude (decimal degrees) -70/+70
     -lon Tx Longitude (decimal degrees) -180/+180
     -rla (Optional) Rx Latitude for PPA (decimal degrees) -70/+70
     -rlo (Optional) Rx Longitude for PPA (decimal degrees) -180/+180
     -f Tx Frequency (MHz) 20MHz to 100GHz (LOS after 20GHz)
     -erp Tx Total Effective Radiated Power in Watts (dBd) inc Tx+Rx gain. 2.14dBi = 0dBd
     -gc Random ground clutter (feet/meters)
     -m Metric units of measurement
     -te Terrain code 1-6 (optional - 1. Water, 2. Marsh, 3. Farmland,
          4. Mountain, 5. Desert, 6. Urban
     -terdic Terrain dielectric value 2-80 (optional)
     -tercon Terrain conductivity 0.01-0.0001 (optional)
     -cl Climate code 1-7 (optional - 1. Equatorial 2. Continental subtropical
          3. Maritime subtropical 4. Desert 5. Continental temperate
          6. Maritime temperate (Land) 7. Maritime temperate (Sea)
     -rel Reliability for ITM model (% of 'time') 1 to 99 (optional, default 50%)
     -conf Confidence for ITM model (% of 'situations') 1 to 99 (optional, default 50%)
     -resample Reduce Lidar resolution by specified factor (2 = 50%)
     -segments Number of segments to divide the plot rectangle into (must be even and > 4)
Output:
     -o basename (Output file basename - required, min 5 chars)
     -dbm Plot Rxd signal power instead of field strength in dBuV/m
     -rt Rx Threshold (dB / dBm / dBuV/m)
     -R Radius (miles/kilometers)
     -res Pixels per tile. 300/600/1200/3600 (Optional. LIDAR res is within the tile)
     -pm Propagation model. 1: ITM, 2: LOS, 3: Hata, 4: ECC33,
          5: SUI, 6: COST-Hata, 7: FSPL, 8: ITWOM, 9: Ericsson,
          10: Plane earth, 11: Egli VHF/UHF, 12: Soil
     -pe Propagation model mode: 1=Urban,2=Suburban,3=Rural
     -ked Knife edge diffraction (Already on for ITM)
Antenna:
     -ant (antenna pattern file basename+path for .az and .el files)
     -txh Tx Height (above ground)
     -rxh Rx Height(s) (optional. Default=0.1)
     -rxg Rx gain dBd (optional for PPA text report)
     -hp Horizontal Polarisation (default=vertical)
     -rot  (  0.0 - 359.0 degrees, default 0.0) Antenna Pattern Rotation
     -dt   ( -10.0 - 90.0 degrees, default 0.0) Antenna Downtilt
     -dtdir ( 0.0 - 359.0 degrees, default 0.0) Antenna Downtilt Direction
Debugging:
     -t Terrain greyscale background
     -dbg Verbose debug messages
     -ng Normalise Path Profile graph
     -haf Halve 1 or 2 (optional)
     -nothreads Turn off threaded processing
     -rp Use experimental radial processing
```

### REFERENCE DATA
Signal server is designed for most of the environments and climates on Planet Earth but Polar region support is limited above extreme latitudes. (Svalbard is ok).

 It can run with or without terrain data and can even be used to simulate radiation of other EM emissions like light.

#### -sdf 
##### Directory containing Digital Elevation Models (DEM)
SDF formatted tiles can be created by converting SRTM tiles (30m or 90m) in HGT format with the srtm2sdf or srtm2sdf-hd utility. At the time of writing these tiles can be obtained for free from the [Viewfinder Panoramas website](http://viewfinderpanoramas.org/dem3.html).

Note these can be compressed using gzip or bzip2 if desired to save disk space and speed up loading the files.  For hi-res (HD) SRTM1 (30m) data, bzip2 compresses best, and for lo-res SRTM3 (90m) data, gzip seems to achieve the best compression.  Either will work fine for whichever data format and resolution is used.

##### WARNING - BREAKING CHANGES
Since 1st Oct 2023, .sdf filenames have been reformatted to address issue #35, replacing colons with underscores to enable compatibility with Python etc. The conversion utility has been updated so .hgt files may require reprocessing, or you can just rename files to replace : with _.


#### -lid 
##### WGS84 ASCII grid tile (LIDAR) with dimensions and resolution defined in header
LIDAR data can be used providing it is in ASCII grid format with WGS84 projection. Resolutions up to 25cm have been tested. 2m is recommended for a good trade off. Cellsize should be in degrees and co-ordinates must be in WGS84 decimal degrees.

To load multiple tiles use commas eg. -lid tile1.asc,tile2.asc. You can load in different resolution tiles and use -resample to set the desired resolution (limited by data limit).
```
ncols        2454
nrows        1467
xllcorner    -1.475333294357
yllcorner    53.378635095801
cellsize     0.000006170864
NODATA_value 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
```

#### -udt 
##### User defined CSV clutter file
This text file allows you to define buildings with co-ordinates and heights.
Elevations in the UDT file are evaluated and then copied into a temporary file under /tmp.  Then the contents of the temp file are scanned, and if found to be unique, are added to the ground elevations described by the digital elevation data in memory. Height units are determined by appending M for meters or nothing for feet.

There is no special parsing for comments, so any lines of data that don't contain 3 numbers separated by commas are interpreted as a comment.

Format: latitude,longitude,height(M)
```
// This is a comment!
54.555,-2.221,10M
54.555,-2.222,10M
54.555,-2.223,10M
```

### Antenna radiation pattern(s)
Antenna  pattern  data is read from a pair of files having the same base name as the output file (-o), but with  .az  and .el  extensions  for azimuth and elevation patterns.  This name can be overridden with the command line option -ant followed by a base filename or path and base filename for the .az and .el files.  The program will first search for command line specified antenna files with -ant, then it will default to searching for antenna files having the same name as the output file.  
```
045.0
0       0.8950590
1       0.8966406
2       0.8981447
3       0.8995795
4       0.9009535
5       0.9022749
```
The  first  line of the .az file specifies the direction measured clockwise in degrees from  True  North.  This is followed by azimuth headings (0 to 360 degrees) and their associated normalized field patterns (0.000 to 1.000) separated by whitespace.  This direction can be overridden by the command line option -rot followed by a positive direction value from 0 to 360 degrees.  This value will override any direction specified in the first line of the .az file.  If not specified on the command line, the program will default to using the direction from the first line of the .az file.

The  structure of SPLAT! elevation pattern files is slightly different. The first line of the .el file specifies the amount of mechanical  beamtilt  applied  to  the  antenna.   A downward tilt is expressed as a positive angle, while an upward tilt is expressed as a negative angle.  This data is followed by the azimuthal direction of the tilt, 0 to 360 degrees, separated by whitespace.  These can be overridden by the command line options -dt for downtilt and -dtdir for downtilt azimuthal direction.  The options, like -rot above, will override any values embedded in the .el file if specified on the command line.

The remainder of the file consists of elevation angles and their radiation pattern (0.000 to 1.000) values separated by whitespace.  Elevation angles must  be  specified  over  a -10.0  to  +90.0  degree  range.  In  this  example,  the  antenna  is  tilted down 2 degrees towards an azimuth of 045.0 degrees.
```
2.0    045.0
-10.0   0.172
-9.5    0.109
-9.0    0.115
-8.5    0.155
-8.0    0.157
```

## Examples
	
### 90m resolution	
- INPUTS: 900MHz tower at 25m AGL with 5W ERP, 30km radius
- OUTPUTS: 1200 resolution, 30km radius, -90dBm receiver threshold, Longley Rice model
```
signalserver -sdf /data/SRTM3 -lat 51.849 -lon -2.2299 -txh 25 -f 900 -erp 5 -rxh 2 -rt -90 -dbm -m -o test1 -R 30 -res 1200 -pm 1
```
### 30m resolution
- INPUTS: 450MHz tower at 25f AGL with 20W ERP, 10km radius
- OUTPUTS: 3600 resolution, 30km radius, 10dBuV receiver threshold, Hata model
```
signalserverHD -sdf /data/SRTM1 -lat 51.849 -lon -2.2299 -txh 25 -f 450 -erp 20 -rxh 2 -rt 10 -o test2 -R 10 -res 3600 -pm 3
```

### 2m resolution (LIDAR)
- INPUTS: 1800MHz tower at 15m AGL with 1W ERP, 1 km radius
- OUTPUTS: 2m LIDAR resolution, 5km radius, -90dBm receiver threshold, Longley Rice model
```
signalserverLIDAR -lid /data/LIDAR/Gloucester_2m.asc -lat 51.849 -lon -2.2299 -txh 15 -f 1800 -erp 1 -rxh 2 -rt -90 -dbm -m -o test3 -R 1 -pm 1
```
### Scripting
By using wrapper scripts like runsig.sh and genkmz.sh, you can streamline running signalserver by pre-setting some commonly used options in the runsig.sh file.  Those options should be the ones you use every time you run signalserver, like "-m" for metric or "-sdf ./sdf_file_path", for example, so you won't have to specify those options every time you run it.  The genkmz.sh file will convert the output ppm file into a .png with transparent background, then will convert it to a Google Earth Keyhole Markup Language (KML) file, and then it compresses it for size to a (KMZ) file.  Here is an example of using the provided sample runsig.sh and genkmz.sh:
```
### Plot 70cm Service contour, 700W ERP, 300 feet AGL, DB413-B, to 150 mi:
cd output/GoogleEarth
./runsig.sh -lat 42.428889 -lon -87.812500 -txh 300 -f 446.000 -erp 700 -R 150 -res 600 -rel 50 -rt 39 -ant ../../antenna/DB413-B -rot 225 -color ../../color/blue -o example-service | ./genkmz.sh

### Plot 70cm Interference contour, 700W ERP, 300 feet AGL, DB413-B, to 150 mi:
cd output/GoogleEarth
./runsig.sh -lat 42.428889 -lon -87.812500 -txh 300 -f 446.000 -erp 700 -R 150 -res 600 -rel 10 -rt 21 -ant ../../antenna/DB413-B -rot 225 -color ../../color/blue -o example-interference | ./genkmz.sh
```

