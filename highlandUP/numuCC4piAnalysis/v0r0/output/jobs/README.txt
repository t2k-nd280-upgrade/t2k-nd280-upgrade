As it is configured, the script createJobs looks for the files with different FHC or RHC and different configurations and nu modes. For each of this files the script generates a parameter file for testing the different target models.

Description of the shorthand nomenclature:

config0 = Current detector
config2 = Upgrade detector
target1 = FGD-1
target2 = FGD-2
target3 = Super-FGD
nu = numu selection in FHC (antinumu contamination in FHC IS NOT simulated)
antinu = antinumu selection in RHC
nubkg_antinu = numu selection in RHC (numu contamination in RHC IS simulated)

FWD    = muons selected in the TPC after the target
BWD    = muons selected in the TPC before the target
HA     = muons selected in one of the TPCs above/below the target
ECal   = muons selected in ECal
Target = muons selected in Target-only (only for Super-FGD !!!)
woTarget = muons selected in a TPC or ECal
  (eff_{woTarget} = eff_{FWD} + eff_{BWD} + eff_{HA} + eff_{ECal})
wTarget = muons selected in a TPC or ECal or in target
  (eff_{wTarget} = eff_{FWD} + eff_{BWD} + eff_{HA} + eff_{ECal} + eff_{Target})

Currently (due to Mathieu modifications) RHC numu and numubar samples are analyzed together. This is not the case for FHC.
