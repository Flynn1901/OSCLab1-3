port=5678


./sensor_node 15 1 127.0.0.1 $port &
./sensor_node 21 3 127.0.0.1 $port &

./sensor_node 37 2 127.0.0.1 $port &

./sensor_node 132 3 127.0.0.1 $port &

./sensor_node 142 3 127.0.0.1 $port &
sleep 5
killall sensor_node
sleep 30
