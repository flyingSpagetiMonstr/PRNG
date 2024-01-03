echo -e "============================="
echo -e "Running main: "

pushd ..
make rerun
popd

echo -e "============================="
echo -e "Running assess: "
./assess 1000000

serial=alpha-1000
cp ./experiments/AlgorithmTesting/finalAnalysisReport.txt ./stored/$serial-finalAnalysisReport.txt
