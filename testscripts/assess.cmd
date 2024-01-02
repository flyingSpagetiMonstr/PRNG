.\assess 1000000

set serial=min64-noshift-SMAX-1000-test-01
@REM Time cost: 12876 milli seconds

@REM xcopy .\experiments\AlgorithmTesting .\stored\%serial%-report /E /I /Q /Y
copy .\experiments\AlgorithmTesting\finalAnalysisReport.txt .\stored\%serial%-finalAnalysisReport.txt
@REM copy .\data\stream.dat .\stored\%serial%-data
powershell -c "[console]::beep(500, 1000)"
@REM shutdown -s -t 0
@REM ==================================================
@REM Billion: 10 0000 0000
