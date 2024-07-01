			================
			SPLAT! Utilities
			================

Utilities for use with SPLAT! software are found under the
/utils directory.  They include the following:


## BUILD
	cmake .
	make

srtm2sdf
========
The srtm2sdf utility generates Data Files (SDFs) for use with Signal-Server
from STS-99 Space Shuttle Topography Mission (SRTM) elevation data files.
This data is of a much higher quality than that contained in older USGS
Digital Elevation Models of the same resolution.  However, many SRTM
Version 2 elevation models contain data "spikes", "voids", and "wells"
that are the consequence of the radar mapping process.

The srtm2sdf utility has the ability to detect and replace SRTM data
outliers with equivalent usgs2sdf derived SDF data (see usgs2sdf below).
If such data is not available, SRTM outliers are handled either through
adjacent pixel averaging, or by threshold limiting using user-specified
limits.  Of all three methods, the USGS-derived SDF replacement method
yields the best results.

The srtm2sdf utility processes SRTM-3 3-arc second resolution data
or use with Signal-Server operating in standard definition mode.

SRTM-3 Version 2 Elevation Data files may be downloaded from:

    http://viewfinderpanoramas.org/dem3.html

The srtm2sdf utility accepts command-line options as follows:

-d:  used to specify the directory path to the location of usgs2sdf
     derived SDF files that are to be used to replace outliers found
     in the SRTM data file.  The -d option overrides the default path
     specified in your $HOME/.splat_path file.

-n:  used to specify the elevation (in meters) below which SRTM data
     is either replaced with usgs2sdf-derived SDF data, or averaged
     among adjacent elevation data points.  The default threshold for
     the replacement limit is sea-level (0 meters).  Unless elevations
     below sea-level are known to exist for the region being
     processed by the srtm2sdf utility, the -n option need not be
     specified.

Some examples of srtm2sdf use:

    srtm2sdf N40W074.hgt

    srtm2sdf -d /cdrom/sdf N40W074.hgt

    srtm2sdf -d /dev/null N40W074.hgt (/dev/null prevents USGS data
		replacement from taking place)

    srtm2sdf -n -5 N40W074.hgt

In all cases, SDF files are written into the current working directory.

The srtm2sdf utility may also be used to convert 3-arc second SRTM data
in Band Interleaved by Line (.BIL) format for use with SPLAT!  This data 
is available via the web at: http://seamless.usgs.gov/website/seamless/

Once the region of the world has been selected at this site, select the
"Define Download Area By Coordinates" button under "Downloads".  Proceed
to request the download of the region(s) desired in 1 degree by 1 degree
regions only, and make sure bounding coordinates entered fall exactly on
whole numbers of latitude and longitude (no decimal fractions of a degree).

Select the "Add Area" button at the bottom.

On the next screen, select "Modify Data Request".  On the subsequent screen,
de-select the National Elevation Dataset (NED) format and select "SRTM 3 arc
sec - Shuttle Radar Topography Mission [Finished]".  Change the format from
ArcGrid to BIL, and from HTML to TXT.

Select the "Save Changes and Return To Summary" button.

Select the "Download" button, and save the file once it has been sent to
your web browser.

Uncompressing the file will generate a directory containing all files
contained within the downloaded archive.  Move into the directory and
invoke srtm2sdf as described above with the filename having the .bil
extension given as its argument.  Finally, move or copy the generated
.sdf file to your SPLAT! working directory.


srtm2sdf-hd
===========
The srtm2sdf-hd utility operates in an identical manner as srtm2sdf,
but is used to generate HD SDF files from SRTM-1 one-arc second
resolution data files for use with signalserverHD. One-arc second
resolution data files may be downloaded from:

    http://viewfinderpanoramas.org/Coverage%20map%20viewfinderpanoramas_org1.htm


usgs2sdf
========
The usgs2sdf utility takes as an argument the name of an uncompressed
and record delimited Digital Elevation Model Data (DEM) downloaded from
the US Geological Survey, and generates a SPLAT Data File (SDF) compatible
with  signalserver.  usgs2sdf may be invoked manually, or via the
postdownload.sh script.

