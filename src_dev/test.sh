#!/usr/bin/bash

set -euo pipefail
trap 'pkill -P $$' SIGINT SIGTERM ERR

number()
{
    calc -dp "config(\"mode\",\"exp\"),;$1"
}

submit ()
{
    substance=$1
    property=$2
    model=$3
    cond1=$4
    cond1v=$(number "$5")
    cond1u=$6
    cond2=$7
    cond2v=$(number "$8")
    cond2u=$9
    expected=$(number "${10}")

    request='{"jsonrpc":"2.0","id":'$RANDOM',"method":"property/execute_function","params":{"signature": {"substance":"'$substance'","property":"'$property'","model":"'$model'","conditions":["'$cond1'","'$cond2'"]},"arguments":[{"value":'$cond1v',"unit":"'$cond1u'"},{"value":'$cond2v',"unit":"'$cond2u'"}]}}'
    response=$(curl -s http://127.0.0.1:7326 -d "$request")
    failed=$(echo $response | jq .error.message)
    if [[ "$failed" != "null" ]] 
    then
        echo "Request failed:"
        echo $request | jq
        exit 1
    fi
    result=$(echo $response | jq .result.value)
    error=$(number "abs($result - $expected) / $expected")
    succes=$(calc -dp "$error < 1e-6")
    if [[ $succes != 1 ]]
    then
        echo "Request gave back bad result:"
        echo $request | jq
        echo "Got $result, expected $expected"
        exit 2
    fi
}

./server -p 7326 > "test.log" 2>&1 &
SERVER_PID=$!
echo "Server pid is $SERVER_PID"
sleep 0.1;

submit  water  pressure  ISTO_IAPWS_R6  temperature  300.0  K  density  0.9965560e3  kg.m-3  "0.992418352e-1 * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  300.0  K  density  0.1005308e4  kg.m-3  "0.200022515e2  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  300.0  K  density  0.1188202e4  kg.m-3  "0.700004704e3  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  500.0  K  density  0.4350000    kg.m-3  "0.999679423e-1 * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  500.0  K  density  0.4532000e1  kg.m-3  "0.999938125    * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  500.0  K  density  0.8380250e3  kg.m-3  "0.100003858e2  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  500.0  K  density  0.1084564e4  kg.m-3  "0.700000405e3  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  647.0  K  density  0.3580000e3  kg.m-3  "0.220384756e2  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  900.0  K  density  0.2410000    kg.m-3  "0.100062559    * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  900.0  K  density  0.5261500e2  kg.m-3  "0.200000690e2  * 1e6"
submit  water  pressure  ISTO_IAPWS_R6  temperature  900.0  K  density  0.8707690e3  kg.m-3  "0.700000006e3  * 1e6"

kill -s SIGTERM $SERVER_PID
exit 0
