make all
port=5678
clients=8
echo -e "starting gateway "
./sensor_gateway $port $clients &
sleep 3
echo -e 'starting 5 sensor nodes'
./sensor_node 132 6 127.0.0.1 $port &
./sensor_node 142 7 127.0.0.1 $port &
./sensor_node 15 1 127.0.0.1 $port &
./sensor_node 21 2 127.0.0.1 $port &
./sensor_node 37 3 127.0.0.1 $port &
./sensor_node 49 2 127.0.0.1 $port &
./sensor_node 112 3 127.0.0.1 $port &
./sensor_node 129 2 127.0.0.1 $port &

sleep 20
killall sensor_node
sleep 30
