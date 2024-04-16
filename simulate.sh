#!/bin/bash

make all

echo -e "Select simulation: "
echo -e "1. 10 processes without compaction"
echo -e "2. 10 processes with compaction"
echo -e "3. 20 processes without compaction"
echo -e "4. 20 processes with compaction"
echo -e "5. 50 processes without compaction"
echo -e "6. 50 processes with compaction"
echo -e "7. 75 processes without compaction"
echo -e "8. 75 processes with compaction"
echo -e "9. 100 processes without compaction"
echo -e "10. 100 processes with compaction"

echo -e "Enter your the number corresponding to your choice: "
read choice

case $choice in
1) echo "Executing 10 processes without compaction \n"
./memorySimulation 10 0
;;

2) echo "Executing 10 processes with compaction \n"
./memorySimulation 10 1
;;

3) echo "Executing 20 processes without compaction \n"
./memorySimulation 20 0
;;

4) echo "Executing 20 processes with compaction \n"
./memorySimulation 20 1
;;

5) echo "Executing 50 processes without compaction \n"
./memorySimulation 50 0
;;

6) echo "Executing 50 processes with compaction \n"
./memorySimulation 50 1
;;

7) echo "Executing 75 processes without compaction \n"
./memorySimulation 75 0
;;

8) echo "Executing 75 processes with compaction \n"
./memorySimulation 75 1
;;

9) echo "Executing 100 processes without compaction \n"
./memorySimulation 100 0
;;

10) echo "Executing 100 processes with compaction \n"
./memorySimulation 100 1
;;

*) echo "Invalid choice"
;;
esac

make clean