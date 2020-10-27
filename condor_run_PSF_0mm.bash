#!/bin/bash

# first argument specified in condor_submit_myAnalysis.condor at point _arguments_ =
CONDOR_CLUSTER_ID=$1 
# second argument specified in condor_submit_myAnalysis.condor at point _arguments_ =
CONDOR_PROCESS_ID=$2

CONDOR_JOBID=$CONDOR_CLUSTER_ID.$CONDOR_PROCESS_ID

# since getenv is used in condor_submit script HOSTNAME is set back to its proper value here
HOSTNAME=`hostname -s`


# this is the directory where all data will be copied after the job is complete
# it has to be mounted on NTFS == can't be a local dir on your machine
OUTPUTDIR=/net/scratch_g4rt/kasper/SimulationRes/SiFiCCProject/PSF/PMMA180MeV0mm/
OUTPUTNAME=result${CONDOR_PROCESS_ID}.root

WORKDIR=${HOME}/geant4_workdir/bin/SPS/
PROGRAM=${WORKDIR}SPS
PRGNAME=SPS
MAC=${WORKDIR}macros/run_0mm.mac
MAC_SRC=${WORKDIR}macros/Run180MeV.mac
MACNAME=run_0mm.mac
LOGFILE=logfile_${CONDOR_JOBID}-${HOSTNAME}.log
SEED=`md5sum /proc/stat|cut -d / -f 1|cut -d ' ' -f 1`


# while the job is executed on a machine within the cluster, the results will be stored temporarily in TMP_DATA_PATH
# usage of /user/scratch/ is mandatory for Physikzentrum
TMP_DATA_PATH=/user/scratch/kasper/kasper-condor-${CONDOR_JOBID}

# Disable core dumps
ulimit -c 0

# Create working directory and go there
if [ -e ${TMP_DATA_PATH} ]; then
   rm -rf ${TMP_DATA_PATH}
fi
mkdir -p ${TMP_DATA_PATH}
if [[ $? == 0 ]]
then
        cd ${TMP_DATA_PATH};
        date >> ${LOGFILE};
        echo "RUN-SCRIPT : Created working directory..." >> ${LOGFILE};

# Copy data file and libs here so they can be read by executable
# use crowd control copy (see /net/software_t2k/tools/cccp/)
        cccp ${PROGRAM} ./
        cccp ${MAC} ./
        cccp ${MAC_SRC} ./
        echo "RUN-SCRIPT : copied  "${PROGRAM}" and "${MAC}" and "${MAC_SRC}" to "${TMP_DATA_PATH}>> ${LOGFILE};

# Run and save program output in logfile
        echo "RUN-SCRIPT : now starting the program " >> ${LOGFILE};
        ./${PRGNAME} ${MACNAME} -s ${SEED} -b 2>&1 >> ${LOGFILE}; 
        mv Out_run*.root ${OUTPUTNAME};

# Save return value
        RES=$?
        echo "RUN-SCRIPT : return value is " $RES >> ${LOGFILE};

# Clean up
        echo "RUN-SCRIPT : Moving local temporary data ( "${LOGFILE}" and " ${OUTPUTNAME}" ) to "${OUTPUTDIR} " ..." >> ${LOGFILE};
        echo "RUN-SCRIPT : Removing local temporary data..." >> ${LOGFILE};
        date >> ${LOGFILE}
        if cp ${LOGFILE} ${OUTPUTNAME} ${OUTPUTDIR};
        then
            rm -rf ${TMP_DATA_PATH};
        fi
else
    RES=1
fi

# Return exit status of actual program run above
exit $RES
