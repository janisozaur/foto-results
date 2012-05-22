#!/bin/bash

for numPhotons in {100..25000..1000}
do
	for numAssociatedPhotons in {5..70..10}
	do
		for streamRadius in {1..50..5}
		do
			resultFile="render_"$numPhotons"_"$numAssociatedPhotons"_"$streamRadius
			./fotorealistyczna $numPhotons 1000 $numAssociatedPhotons $streamRadius $resultFile
		done
	done

done
