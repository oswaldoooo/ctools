#!/bin/bash
if [[ "$CODELAB" == "" ]];then
	if [[ "$CTOOLS" == "" ]];then
		echo "not set variable,build failed"
		exit 1
	else
		if [[ -d $CTOOLS/tools ]];then
			cd $CODELAB/tools && bash build.sh
		else
			echo "$CTOOLS/tools not exist"
		fi
		if [[ -d $CTOOLS/parser ]];then
			cd $CODELAB/parser && bash build.sh
		else
			echo "$CTOOLS/parser not exist"
		fi
		echo "build done"
	fi
else
	CTOOLS="$CODELAB/ctools"
	if [[ -d $CTOOLS/tools ]];then
		cd $CTOOLS/tools && bash build.sh
	else
		echo "$CTOOLS/tools not exist"
	fi
	if [[ -d $CTOOLS/parser ]];then
		cd $CTOOLS/parser && bash build.sh
	else
		echo "$CTOOLS/parser not exist"
	fi
	echo "build done"
fi
