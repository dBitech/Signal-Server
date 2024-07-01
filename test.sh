#!/bin/bash

echo "*************************************************************************"
echo "Set DEM directory..."
demDir=data
echo "DEM directory:  ${demDir}"

echo "Prepare test directory..."
testDir=output/tests

echo "test directory:  ${testDir}"

if [ -d "${testDir}" ];
then
	rm -R ${testDir};
fi;

mkdir ${testDir};


echo "*************************************************************************"
echo "Running 50cm LIDAR test..."
time ./build/signalserverLIDAR -lid data/sk3587_50cm.asc -lat 53.383 -lon -1.468 -txh 8 -f 446 -erp 1 -rxh 2 -m -dbm -rt -90 -o ${testDir}/1_Original -R 0.5 -t
echo "Converting to PNG..."
convert ${testDir}/1_Original.ppm -transparent white -channel Alpha PNG32:${testDir}/1_Original.png
rm ${testDir}/1_Original.ppm
rm ${testDir}/1_Original.*cf

echo "*************************************************************************"
echo "Running 30m Meridian test..."
time ./build/signalserverLIDAR -lid data/N051E000_AVE_DSM.tif.asc,data/N051W001_AVE_DSM.tif.asc -lat 51.472 -lon 0.0096 -txh 12 -f 446 -erp 5 -rxh 2 -m -dbm -rt -100 -o ${testDir}/2_Original -R 10 -t
echo "Converting to PNG..."
convert ${testDir}/2_Original.ppm -transparent white -channel Alpha PNG32:${testDir}/2_Original.png
rm ${testDir}/2_Original.ppm
rm ${testDir}/2_Original.*cf

echo "*************************************************************************"
echo "Commencing REGACOM tests..."
cp output/OpenLayers/Tests/Regacom_Tests_WGS84.html ${testDir}/

echo "*************************************************************************"
echo "Running 446 Mhz Antenna and UDT Clutter test..."
time ./build/signalserver -sdf ${demDir} -lat 42.328889 -lon -87.862500 -txh 300 -rxh 2 -f 446 -erp 700 -R 50 -res 600 -rt 39 -ant antenna/DB413-B -rot 180 -udt data/test.udt -pm 1 -t -o ${testDir}/ant-udt_test_Original
echo "Converting to PNG..."
convert ${testDir}/ant-udt_test_Original.ppm ${testDir}/ant-udt_test_Original.png
rm ${testDir}/ant-udt_test_Original.ppm
rm ${testDir}/ant-udt_test_Original.*cf

echo "*************************************************************************"
echo "Running ITM coverage map test..."
time ./build/signalserverHD -pm 1 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 40.0 -o ${testDir}/REGACOM_ITM_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_ITM_Original.ppm -transparent white ${testDir}/REGACOM_ITM_Original.png
rm ${testDir}/REGACOM_ITM_Original.ppm


#echo "*************************************************************************"
#echo "Running LOS coverage map test. WARNING: THIS HAS NO CROPPING SO WILL BE A *VERY* BIG IMAGE"
#time ./build/signalserverHD -pm 2 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_LOS_Original
#echo "Converting to PNG..."
#convert ${testDir}/REGACOM_LOS_Original.ppm -transparent white ${testDir}/REGACOM_LOS_Original.png
#rm ${testDir}/REGACOM_LOS_Original.ppm

echo "*************************************************************************"
echo "Running Hata coverage map test..."
time ./build/signalserverHD -pm 3 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_Hata_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_Hata_Original.ppm -transparent white ${testDir}/REGACOM_Hata_Original.png
rm ${testDir}/REGACOM_Hata_Original.ppm

echo "*************************************************************************"
echo "Running ECC33 coverage map test..."
time ./build/signalserverHD -pm 4 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_ECC33_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_ECC33_Original.ppm -transparent white ${testDir}/REGACOM_ECC33_Original.png
rm ${testDir}/REGACOM_ECC33_Original.ppm

echo "*************************************************************************"
echo "Running SUI coverage map test..."
time ./build/signalserverHD -pm 5 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_SUI_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_SUI_Original.ppm -transparent white ${testDir}/REGACOM_SUI_Original.png
rm ${testDir}/REGACOM_SUI_Original.ppm


echo "*************************************************************************"
echo "Running COST-Hata coverage map test..."
time ./build/signalserverHD -pm 6 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_COST-Hata_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_COST-Hata_Original.ppm -transparent white ${testDir}/REGACOM_COST-Hata_Original.png
rm ${testDir}/REGACOM_COST-Hata_Original.ppm

echo "*************************************************************************"
echo "Running FSPL coverage map test..."
time ./build/signalserverHD -pm 7 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_FSPL_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_FSPL_Original.ppm -transparent white ${testDir}/REGACOM_FSPL_Original.png
rm ${testDir}/REGACOM_FSPL_Original.ppm


echo "*************************************************************************"
echo "Running ITWOM coverage map test..."
time ./build/signalserverHD -pm 8 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 40.0 -o ${testDir}/REGACOM_ITWOM_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_ITWOM_Original.ppm -transparent white ${testDir}/REGACOM_ITWOM_Original.png
rm ${testDir}/REGACOM_ITWOM_Original.ppm


echo "*************************************************************************"
echo "Running Ericsson coverage map test..."
time ./build/signalserverHD -pm 9 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_Ericsson_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_Ericsson_Original.ppm -transparent white ${testDir}/REGACOM_Ericsson_Original.png
rm ${testDir}/REGACOM_Ericsson_Original.ppm


echo "*************************************************************************"
echo "Running Egli coverage map test..."
time ./build/signalserverHD -pm 11 -sdf ${demDir} -lat 46.977815 -lon 7.528691 -txh 40.0 -erp 4.0 -f 161.3 -rxh 1.5 -dbm -rt -90.0 -m -R 50.0 -o ${testDir}/REGACOM_Egli_Original
echo "Converting to PNG..."
convert ${testDir}/REGACOM_Egli_Original.ppm -transparent white ${testDir}/REGACOM_Egli_Original.png
rm ${testDir}/REGACOM_Egli_Original.ppm

echo "TESTS COMPLETE"