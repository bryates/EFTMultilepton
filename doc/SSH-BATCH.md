#Batch mode

## 1. Make trees

To make trees:

   	source crabscript.csh
	./launchsshbatchjobs.sh

## 2. Make histograms

To make and hadd histograms:

   	cd EFTMultilepton/DrawPlots
	./launchmakehistos.sh
   	./haddhistos.sh

## 3. Make stack plots

To make stack plots from histograms:

      cd EFTMultilepton/DrawPlots
      ./DrawStackPlots.py -w stack_plot_configuration.yaml
