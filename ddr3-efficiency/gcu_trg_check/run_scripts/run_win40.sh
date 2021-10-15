echo "Starting analysis for 0.8k"
for file in ../../gcu2-12_ch012_800Hz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 800
done
echo "Starting analysis for 1k"
for file in ../../gcu2-12_ch012_1kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 1000
done
echo "Starting analysis for 3k"
for file in ../../gcu2-12_ch012_3kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 3000
done
echo "Starting analysis for 5k"
for file in ../../gcu2-12_ch012_5kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 5000
done
echo "Starting analysis for 7k"
for file in ../../gcu2-12_ch012_7kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 7000
done
echo "Starting analysis for 9k"
for file in ../../gcu2-12_ch012_9kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 9000
done
echo "Starting analysis for 10k"
for file in ../../gcu2-12_ch012_10kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 10000
done
echo "Starting analysis for 13k"
for file in ../../gcu2-12_ch012_13kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 13000
done
echo "Starting analysis for 20k"
for file in ../../gcu2-12_ch012_20kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 20000
done
echo "Starting analysis for 30k"
for file in ../../gcu2-12_ch012_30kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 30000
done
echo "Starting analysis for 40k"
for file in ../../gcu2-12_ch012_40kHz_win40_GCU2048/*
do ../gcu-check_v1 "$file" 40 -o rateGCU_win40.txt -x 40000
done
