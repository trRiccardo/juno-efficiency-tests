echo "Starting analysis for 1k"
for file in ../../1kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 1000
done
echo "Starting analysis for 5k"
for file in ../../5kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 5000
done
echo "Starting analysis for 10k"
for file in ../../10kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 10000
done
echo "Starting analysis for 12k"
for file in ../../12kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 12000
done
echo "Starting analysis for 13k"
for file in ../../13kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 13000
done
echo "Starting analysis for 14k"
for file in ../../14kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 14000
done
echo "Starting analysis for 15k"
for file in ../../15kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 15000
done
echo "Starting analysis for 20k"
for file in ../../20kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 20000
done
echo "Starting analysis for 30k"
for file in ../../30kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 30000
done
echo "Starting analysis for 40k"
for file in ../../40kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 40000
done
echo "Starting analysis for 50k"
for file in ../../50kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 50000
done
echo "Starting analysis for 100k"
for file in ../../100kHz_win40/* 
do ../gcu-check_v1 "$file" 40 -o rate_ihep_win40.txt -x 100000
done
