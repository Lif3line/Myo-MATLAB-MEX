# Myo-MATLAB Interface (MEX)

----
## Intro
Interface demonstration for taking Myo EMG data in near real time and piping to MATLAB; script shows the data on a graph in 40 sample long clips. utilises a MEX interface. See also [the non-MEX version](https://github.com/Lif3line/Myo-MATLAB-Interface) which outputs to a csv file which is easier for data gathering.

* Distributed under a modified MIT Licence
	* For academic works please reference with the following key information: "A, Hartwell (2016) Myo-MATLAB Interface"

* Requires Windows (OS based high res timestamps - 100ns resolution).

* Build using Myo SDK 0.9 and Myo Connect 1.01 

----
## Usage
1. Get Myo Connect and ensure running correctly 
2. Ensure you have Microsoft Visual C++ REdistributables for Visual Studio 2015: https://www.microsoft.com/en-gb/download/details.aspx?id=48145
3. Run myoTestEMG.m

----
## Notes on Visual Studio Project
If recompiling the project you'll have to do some additional set up rather than just opening the project; see [Myo SDK Website](https://developer.thalmic.com/docs/api_reference/platform/the-sdk.html) for further details of the SDK and how to use it as this will depend on operating system/compiler etc.

# Have Fun ^^
