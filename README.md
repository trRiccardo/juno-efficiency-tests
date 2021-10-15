# juno-efficiency-tests

Gnuplot scripts, C++ programs and ROOT macros used in the efficiency characterization of the JUNO test facility set up in Legnaro, Italy, at Legnaro National Laboratories (INFN).
General information:

```bash
# where to find data
cd /data/48PMT/tier1/<date>/<subdirectory>
# analysis scripts
cd /data/48PMT/gcu-proc-dev/analysis
# remember to enable root before the analysis
scl enable devtoolset-8 bash
swmod load root
```

## Part 1: Raw Data Processing

Raw data are processed using ```gcu-proc```, a dedicated C++ program which analyzes binary files and outputs ```.root``` files storing all acquired information in ```TTree``` structures.
Initialize the analysis creating the ```.json``` file with acquisition parameters.

```bash
cd /data/48PMT
gcu-proc-dev/scripts/tier1-init.sh <date>/<directory> <run>
# edit json settings file (trigger window; integration window)
nano /data/48PMT/tier1/<date>/<run>/settings.json 
```

Analyize the binary files using *single channel analysis* or *event building analysis*.

```bash
# single channel
cd /data/48PMT
gcu-proc-dev/scripts/tier1-spawn-single.sh <date> <run>

# event building
cd /data/48PMT
gcu-proc-dev/scripts/tier1-spawn.sh <date> <run>
# suggested: sleep(10)

# check analysis status
ps aux | grep gcu-proc
# check data
cd /data/48PMT/tier1/<date>/<run>
```

It is suggested to use bash cycles in case of an high number of input files.

```bash
cd /data/48PMT
# initialize the analysis
for d in $(ls 20210716/gcu0-19_15ch_CH2048/); do gcu-proc-dev/scripts/tier1-init.sh 20210716/gcu0-19_15ch_CH2048/ $d; done
# aanalysis
for d in $(ls 20210716/gcu0-19_15ch_CH2048/); do gcu-proc-dev/scripts/tier1-spawn.sh 20210716/gcu0-19_15ch_CH2048/ $d; sleep 10; done
```

## Part 2: Efficiency Analysis

Compute the efficiency of the IPbus data transmission through *event building analysis*. This part is carried out through a ROOT macro, which processes the ```TTree``` structures.

```bash
# load the macro
root
.L /data/48PMT/gcu-proc-dev/analysis/tndiff.C

# in ROOT:
tndiff_single( freq, nch, "directory" )
# e.g.: tndiff_single( 1000, 3, "1kHz_win40" )
```

Useful bash lines to further process output files:

```bash
# concatenate data files for plot
cat events_lost_Nch*.dat >> eventBuilding_unsorted.dat
sort -n eventBuilding_unsorted.dat >> eventBuilding.dat
rm eventBuilding_unsorted.dat
```
Data are formatted in the following way:

```bash
# survival_per_channel*.dat
freq ok/lost(ch1) ok/tot(ch2) ok/tot(ch3) ok/tot

# events_lost_Nch*.dat
freq ok(0 ch lost) ok/tot(0 ch lost) ok(1 ch lost) ok/tot(1 ch lost) ok(2 ch lost) ok/tot(2 ch lost) evPerCh-Ev (totEv/nCh - Ev)/totEv/nCh
```

## Part 3: DDR3 Tests

Follow the instructions below to prepare the setup for a DDR3 acquisition with 1 GCU in the Padua facility.
1. Turn on **DT5810D Emulator** and plug usb cable from pc to emulator;
2. Open the **Detector Simulator 2019** app on pc and double tap on red button
3. Open **OneTouch** panel. Prepare setup (example values below) and click to **enable output**.
    - Energy. LSB: 4001; V: 1;
    - Time base. Variable rate.
    - Shape. Values for a negative pulse of approximately 40 ns. Rise: 0.01 us; Tau: 0.02 us.
    - Shape. Rise: 0.01 us; Tau: 0.005 us.
    - On General Settings disable "Analog Filter" and enable "Emulator", "FAST", "Hi-Z".
4. Plug emulator to GCU (channel 2)
5. Enable the DDR3 for the first time.

```bash
# acquisition:
cd gcu1f3/gcu_v2_scripts/gcu_acq_scripts/ddr
python enable_ddr.py -s<number-of-GCUs>
```
In this way the acquisition continues automatically and the content is overwritten until the memory is readout.
It is possible to estimate the waiting time by diving 2 GBs by the trigger window (expressed in Bytes) and by the rate set prior to the acquisition.
6. Finally acquire data and readout.
```bash
# readout:
python ddr_readout.py -s<number-of-GCUs> -b True -f <path>/<fname>
# continue process in background with CTRL + Z 
bg
# check dimension until ddr3 file has 2 GB dim  
ls -lhrt
```

## Part 4: DDR3 Efficiency Analysis

Process the raw data using a similar tool as before. Firstly edit the ```.json``` file accordingly.

```bash
nano settings.json
# "window" : 0 -> ( tw - 2 ) * 8
# "integration" : 60 -> 250
# "swap-words" : false
# "ddr3" : generally true, false on the preliminary version (1 channel, reversed files)
# "verbose" : generally 0, 2 for debugging (buffer dumped on screen del buffer, analyze with "-v 2") 
```
At this point, exploit the dedicated DDR3-version of ```gcu-proc``` for the analysis.

```bash
# settings from json file: --json <file>.json; verbose: -v 2; number of events dumped: -n 2; 
# in the old version (needed for ddr3 data analysis) there is no need for the "-w" option (linked to waveforms)
~/Software/gcu-proc/gcu2tier1-ddr3 -f <input-file> -o <output-file> --json settings.json -n 2 -v 2
# the output file will be <output-file>.root

# once sure about parameters, do:
~/Software/gcu-proc/gcu2tier1-ddr3 -f <input-file> -o <output-file> --json settings.json --out-file-size-Gb 3
# --out-file-size-Gb: max <file>.root size in Gb (size is approximately 1.8 Gb in this case)

# analyze with event building to get survival fraction
root
.L tndiff.C
tndiff_single( <rate>, <number-of-chs>, "<dir>" );
# other used tools: gcu_trg_check, timeRate.C
# at the moment only timeRate.C is successful for 1 channel 1 GCU DDR3 data
```

Finally check the waveforms' most meaningful parameters such as the *trigger number* and the *timestamp* trend as function of the event number, using the ROOT output files.

```c++
# plot waveforms to check data quality
root
.L /data/48PMT/gcu-proc-dev/analysis/plot_waveforms.C
plots_waveforms( "<file>.root", 0, 10, 40 );
# int ch (0): channel, always 0 in certain versions; int n (10): number of waveforms; int nsample (40): trigger window * 8

# check tree parameters
root -l <file>.root
.ls
eventTree->Print();
# plot timestamp and trigger number *sequentially* to check status
eventTree->Draw( "trg_number:Entry$" );
eventTree->Draw( "ref_timestamp:Entry$" );
```





