#!/bin/bash
# Check if out.hipo exists and remove it if it does
if [ -f "./out/out.hipo" ]; then
    rm "out.hipo"
    echo "out.hipo has been removed."
else
    echo "out.hipo does not exist."
fi

# Check if recon.out.hipo exists and remove it if it does
if [ -f "./out/recon.out.hipo" ]; then
    rm "recon.out.hipo"
    echo "recon.out.hipo has been removed."
else
    echo "recon.out.hipo does not exist."
fi

##########################################################################################################################################
# Step 1. Run Richard's epi+n generator
root -b -q -l eventGen.cpp # --> Saves file in ./out/epipn_0000.txt  (default is 1 file with 1000 'events')


# Step 2. Run `gemc` on the 'epipn_####.txt' file
# Step 2a. Copy over the 'rga_fall2018.gcard'
cp /group/clas12/gemc/5.1/config/rga_fall2018.gcard ./
# Step 2b. Remove problematic rich lines
sed -i '/<detector name="experiments\/clas12\/rich_sector4\/rich"     factory="TEXT" variation="java"\/>/d' "rga_fall2018.gcard"
sed -i '/<detector name="experiments\/clas12\/rich_sector4\/javacad\/" factory="CAD"\/>/d' "rga_fall2018.gcard"
# Step 2c. Run `gemc`
gemc rga_fall2018.gcard -USE_GUI=0 -INPUT_GEN_FILE="LUND, ./out/epipn_0000.txt" -OUTPUT="hipo, out/out.hipo"
# --> Saves file in ./out/out.hipo


# Step 3. Run `recon-util` on the './out/out.hipo' file
recon-util -i out/out.hipo -o out/recon.out.hipo -y /group/clas12/gemc/5.1/config/rga_fall2018.yaml
# --> Saves file in ./out/recon.out.hipo


# Step 4. Run `clas12root read_rec_particle.cpp` on the './out/recon.out.hipo' file
clas12root read_rec_particle.cpp 





