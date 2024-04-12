#!/usr/bin/bash

set -euo pipefail
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

number()
{
    calc -dp "config(\"mode\",\"exp\"),;$1"
}

validate_request ()
{
    r="$1"
    set +e
    echo "$r" | jq >&/dev/null
    if [[ $? -ne 0 ]]
    then
        echo "Invalid JSON:"
        echo "$r"
        exit 1
    fi
    set -e
}

should_succeed ()
{
    substance="$1"
    property="$2"
    model="$3"
    cond1="$4"
    cond1v=$(number "$5")
    cond1u="$6"
    cond2="$7"
    cond2v=$(number "$8")
    cond2u="$9"
    expected=$(number "${10}")

    request='{"jsonrpc":"2.0","id":'$RANDOM',"method":"property/execute_function","params":{"signature":{"substance":"'$substance'","property":"'$property'","model":"'$model'","conditions":["'$cond1'","'$cond2'"]},"arguments":{"'$cond1'":{"value":'$cond1v',"unit":"'$cond1u'"},"'$cond2'":{"value":'$cond2v',"unit":"'$cond2u'"}}}}'
    validate_request "$request"
    response=$(curl -s http://127.0.0.1:7326 -d "$request")
    failed=$(echo "$response" | jq .error.message)
    if [[ "$failed" != "null" ]] 
    then
        echo "Request failed:"
        echo "$request" | jq
        echo "Response was:"
        echo "$response" | jq
    fi
    result=$(echo "$response" | jq .result.value)
    error=$(number "abs(abs($result - $expected) / $expected)")
    succes=$(calc -dp "$error < 1e-5")
    if [[ "$succes" != 1 ]]
    then
        echo "Request gave back bad result:"
        echo "$request" | jq
        echo "Got $result, expected $expected"
    fi
}

should_fail ()
{
    error_code=$1
    function=$2
    params=$3

    request='{"jsonrpc":"2.0","id":'$RANDOM',"method":"property/'$function'","params":{'$params'}}'
    validate_request "$request"
    echo "$request"

    response=$(curl -s http://127.0.0.1:7326 -d "$request")
    echo "$response" | jq

    error=$(echo "$response" | jq .error.code)
    if [[ "$error" -ne "$error_code" ]]
    then
        echo "Request should have failed with error code $error_code:"
        #echo $request | jq
        echo "Response was:"
        echo "$response" | jq
        exit 1
    fi
}

LD_LIBRARY_PATH=../src ../src/server -p 7326 > "test.log" 2>&1 &
SERVER_PID=$!
echo "Server pid is $SERVER_PID"
# Give the server some time to initialize everything
sleep 0.1;


should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            temperature       900              K        density           0.241              kg.m-3       9166.53194
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           0.241            kg.m-3   temperature       900                K            9166.53194

should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           0.435            kg.m-3   temperature       500                K            7944.88271
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           1005.308         kg.m-3   temperature       300                K            387.405401
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           1084.564         kg.m-3   temperature       500                K            2032.37509
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           1188.202         kg.m-3   temperature       300                K            132.60961600000002
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           358              kg.m-3   temperature       647                K            4320.92307
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           4.532            kg.m-3   temperature       500                K            6825.02725
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           52.615           kg.m-3   temperature       900                K            6590.702249999999
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           838.025          kg.m-3   temperature       500                K            2566.90919
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           870.769          kg.m-3   temperature       900                K            4172.23802
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6            density           996.556          kg.m-3   temperature       300                K            393.062643
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           0.241            kg.m-3   temperature       900                K            1758.9065699999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           0.435            kg.m-3   temperature       500                K            1508.17541
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           1005.308         kg.m-3   temperature       300                K            4067.9834699999997
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           1084.564         kg.m-3   temperature       500                K            3074.37693
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           1188.202         kg.m-3   temperature       300                K            3461.3558000000003
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           358              kg.m-3   temperature       647                K            6183.157279999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           4.532            kg.m-3   temperature       500                K            1669.9102500000001
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           52.615           kg.m-3   temperature       900                K            1935.10526
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           838.025          kg.m-3   temperature       500                K            3221.06219
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           870.769          kg.m-3   temperature       900                K            2664.2235
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6            density           996.556          kg.m-3   temperature       300                K            4130.181119999999
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           0.241            kg.m-3   temperature       900                K            100062.559
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           0.435            kg.m-3   temperature       500                K            99967.9423
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           1005.308         kg.m-3   temperature       300                K            20002251.5
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           1084.564         kg.m-3   temperature       500                K            700000405
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           1188.202         kg.m-3   temperature       300                K            700004704
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           358              kg.m-3   temperature       647                K            22038475.6
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           4.532            kg.m-3   temperature       500                K            999938.125
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           52.615           kg.m-3   temperature       900                K            20000069
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           838.025          kg.m-3   temperature       500                K            10000385.8
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           870.769          kg.m-3   temperature       900                K            700000006
should_succeed   water               pressure                         ISTO_IAPWS_R6            density           996.556          kg.m-3   temperature       300                K            99241.8352
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           0.241            kg.m-3   temperature       900                K            724.027147
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           0.435            kg.m-3   temperature       500                K            548.314253
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           1005.308         kg.m-3   temperature       300                K            1534.92501
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           1084.564         kg.m-3   temperature       500                K            2412.00877
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           1188.202         kg.m-3   temperature       300                K            2443.57992
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           358              kg.m-3   temperature       647                K            252.145078
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           4.532            kg.m-3   temperature       500                K            535.739001
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           52.615           kg.m-3   temperature       900                K            698.445674
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           838.025          kg.m-3   temperature       500                K            1271.28441
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           870.769          kg.m-3   temperature       900                K            2019.33608
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6            density           996.556          kg.m-3   temperature       300                K            1501.51914
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          10000385.8       Pa       temperature       500                K            838.025
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          100062.559       Pa       temperature       900                K            0.241
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          20000069         Pa       temperature       900                K            52.615
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          20002251.5       Pa       temperature       300                K            1005.308
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          22038475.6       Pa       temperature       647                K            358
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          700000006        Pa       temperature       900                K            870.769
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          700000405        Pa       temperature       500                K            1084.564
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          700004704        Pa       temperature       300                K            1188.202
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          99241.8352       Pa       temperature       300                K            996.556
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          99967.9423       Pa       temperature       500                K            0.435
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    pressure          999938.125       Pa       temperature       500                K            4.532
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          20002251.5         Pa           1005.308
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          700004704          Pa           1188.202
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          99241.8352         Pa           996.556
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          10000385.8         Pa           838.025
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          700000405          Pa           1084.564
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          99967.9423         Pa           0.435
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          999938.125         Pa           4.532
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       647              K        pressure          22038475.6         Pa           358
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          100062.559         Pa           0.241
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          20000069           Pa           52.615
should_succeed   water               density                          ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          700000006          Pa           870.769
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          10000385.8       Pa       temperature       500                K            2566.90919
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          100062.559       Pa       temperature       900                K            9166.53194
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          20000069         Pa       temperature       900                K            6590.702249999999
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          20002251.5       Pa       temperature       300                K            387.405401
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          22038475.6       Pa       temperature       647                K            4320.92307
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          700000006        Pa       temperature       900                K            4172.23802
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          700000405        Pa       temperature       500                K            2032.37509
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          700004704        Pa       temperature       300                K            132.60961600000002
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          99241.8352       Pa       temperature       300                K            393.062643
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          99967.9423       Pa       temperature       500                K            7944.88271
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    pressure          999938.125       Pa       temperature       500                K            6825.02725
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          20002251.5         Pa           387.405401
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          700004704          Pa           132.60961600000002
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          99241.8352         Pa           393.062643
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          10000385.8         Pa           2566.90919
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          700000405          Pa           2032.37509
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          99967.9423         Pa           7944.88271
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          999938.125         Pa           6825.02725
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       647              K        pressure          22038475.6         Pa           4320.92307
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          100062.559         Pa           9166.53194
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          20000069           Pa           6590.702249999999
should_succeed   water               massic_entropy                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          700000006          Pa           4172.23802
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          10000385.8       Pa       temperature       500                K            3221.06219
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          100062.559       Pa       temperature       900                K            1758.9065699999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          20000069         Pa       temperature       900                K            1935.10526
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          20002251.5       Pa       temperature       300                K            4067.9834699999997
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          22038475.6       Pa       temperature       647                K            6183.157279999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          700000006        Pa       temperature       900                K            2664.2235
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          700000405        Pa       temperature       500                K            3074.37693
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          700004704        Pa       temperature       300                K            3461.3558000000003
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          99241.8352       Pa       temperature       300                K            4130.181119999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          99967.9423       Pa       temperature       500                K            1508.17541
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    pressure          999938.125       Pa       temperature       500                K            1669.9102500000001
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          20002251.5         Pa           4067.9834699999997
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          700004704          Pa           3461.3558000000003
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          99241.8352         Pa           4130.181119999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          10000385.8         Pa           3221.06219
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          700000405          Pa           3074.37693
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          99967.9423         Pa           1508.17541
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          999938.125         Pa           1669.9102500000001
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       647              K        pressure          22038475.6         Pa           6183.157279999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          100062.559         Pa           1758.9065699999999
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          20000069           Pa           1935.10526
should_succeed   water               massic_isochoric_heat_capacity   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          700000006          Pa           2664.2235
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          10000385.8       Pa       temperature       500                K            1271.28441
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          100062.559       Pa       temperature       900                K            724.027147
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          20000069         Pa       temperature       900                K            698.445674
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          20002251.5       Pa       temperature       300                K            1534.92501
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          22038475.6       Pa       temperature       647                K            252.145078
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          700000006        Pa       temperature       900                K            2019.33608
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          700000405        Pa       temperature       500                K            2412.00877
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          700004704        Pa       temperature       300                K            2443.57992
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          99241.8352       Pa       temperature       300                K            1501.51914
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          99967.9423       Pa       temperature       500                K            548.314253
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    pressure          999938.125       Pa       temperature       500                K            535.739001
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          20002251.5         Pa           1534.92501
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          700004704          Pa           2443.57992
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       300              K        pressure          99241.8352         Pa           1501.51914
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          10000385.8         Pa           1271.28441
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          700000405          Pa           2412.00877
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          99967.9423         Pa           548.314253
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       500              K        pressure          999938.125         Pa           535.739001
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       647              K        pressure          22038475.6         Pa           252.145078
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          100062.559         Pa           724.027147
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          20000069           Pa           698.445674
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R6_INVERSE    temperature       900              K        pressure          700000006          Pa           2019.33608
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.254991145e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.333568375e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.521976855e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.115331273e6
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.975542239e6
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.516723514e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.657122604e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.263149474e7
should_succeed   water               massic_enthalpy                  ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.184142828e6
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.852238967e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.101749996e5
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.965408875e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.392294795e3
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.258041912e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.772970133e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.853640523e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.517540298e4
should_succeed   water               massic_entropy                   ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.368563855e3
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.241169160e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.301262819e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.452749310e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.112324818e6
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.971934985e6
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.447495124e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.563707038e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.246861076e7
should_succeed   water               massic_internal_energy           ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.106448356e6
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.191300162e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.208141274e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.261609445e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.417301218e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.465580682e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.272724317e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.288569882e4
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.103505092e5
should_succeed   water               massic_isobaric_heat_capacity    ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.401008987e4
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          100e6            Pa       massic_enthalpy   2100e3             J.kg-1       1.676229776e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          100e6            Pa       massic_enthalpy   2700e3             J.kg-1       2.404234998e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20e6             Pa       massic_enthalpy   1700e3             J.kg-1       1.749903962e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20e6             Pa       massic_enthalpy   2500e3             J.kg-1       6.670547043e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50e6             Pa       massic_enthalpy   2000e3             J.kg-1       1.908139035e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50e6             Pa       massic_enthalpy   2400e3             J.kg-1       2.801244590e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          100e6            Pa       massic_entropy    4.0e3              J.kg-1.K-1   1.555893131e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          100e6            Pa       massic_entropy    5.0e3              J.kg-1.K-1   2.449610757e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20e6             Pa       massic_entropy    3.8e3              J.kg-1.K-1   1.733791463e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20e6             Pa       massic_entropy    5.0e3              J.kg-1.K-1   6.262101987e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50e6             Pa       massic_entropy    3.6e3              J.kg-1.K-1   1.469680170e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50e6             Pa       massic_entropy    4.5e3              J.kg-1.K-1   2.332634294e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.394913866e2
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.923015898e2
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.138455090e1
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          17.0e6           Pa       temperature       626.00             K            8.483262001e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          19.1e6           Pa       temperature       635.00             K            1.932829079e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20.0e6           Pa       temperature       630.00             K            1.761696406e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20.0e6           Pa       temperature       638.00             K            1.985387227e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          20.0e6           Pa       temperature       640.00             K            6.227528101e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          21.1e6           Pa       temperature       640.00             K            1.970999272e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          21.1e6           Pa       temperature       644.00             K            5.251009921e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          21.5e6           Pa       temperature       644.60             K            2.268366647e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          21.8e6           Pa       temperature       643.00             K            2.043919161e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          21.8e6           Pa       temperature       648.00             K            5.256844741e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.064e6         Pa       temperature       647.05             K            2.717655648e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.064e6         Pa       temperature       647.15             K            3.701940010e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.0e6           Pa       temperature       646.10             K            2.296350553e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.0e6           Pa       temperature       646.84             K            2.698354719e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.0e6           Pa       temperature       646.89             K            3.798732962e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.11e6          Pa       temperature       648.00             K            4.528072649e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.15e6          Pa       temperature       647.50             K            3.694032281e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.3e6           Pa       temperature       647.90             K            2.811424405e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.3e6           Pa       temperature       648.10             K            3.622226305e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.3e6           Pa       temperature       649.00             K            4.556905799e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.5e6           Pa       temperature       648.60             K            2.832373260e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.6e6           Pa       temperature       646.00             K            2.117860851e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.6e6           Pa       temperature       648.60             K            2.533063780e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.6e6           Pa       temperature       649.00             K            2.923432711e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.6e6           Pa       temperature       649.10             K            3.131208996e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.6e6           Pa       temperature       649.40             K            3.715596186e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.8e6           Pa       temperature       649.30             K            2.572971781e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.8e6           Pa       temperature       649.70             K            2.913311494e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.8e6           Pa       temperature       649.90             K            3.221160278e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          22.8e6           Pa       temperature       650.20             K            3.664754790e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.0e6           Pa       temperature       646.00             K            2.062374674e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.0e6           Pa       temperature       660.00             K            6.109525997e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.5e6           Pa       temperature       655.00             K            4.545001142e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.6e6           Pa       temperature       649.00             K            2.163198378e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.6e6           Pa       temperature       652.00             K            2.651081407e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          23.6e6           Pa       temperature       653.00             K            3.273916816e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          24.0e6           Pa       temperature       650.00             K            2.166044161e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          24.0e6           Pa       temperature       654.00             K            2.967802335e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          24.0e6           Pa       temperature       655.00             K            3.550329864e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          24.0e6           Pa       temperature       660.00             K            5.100267704e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          24.0e6           Pa       temperature       670.00             K            6.427325645e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          26.0e6           Pa       temperature       656.00             K            2.245587720e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          26.0e6           Pa       temperature       661.00             K            2.970225962e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          26.0e6           Pa       temperature       671.00             K            5.019029401e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.100215168e-2
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.120241800e-2
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.0e6           Pa       temperature       650.00             K            1.819560617e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.0e6           Pa       temperature       670.00             K            2.506897702e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.0e6           Pa       temperature       675.00             K            3.004627086e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.0e6           Pa       temperature       690.00             K            4.656470142e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.230761299e-1
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.311385219e-1
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.542946619e-2
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50.0e6           Pa       temperature       630.00             K            1.470853100e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          50.0e6           Pa       temperature       710.00             K            2.204728587e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.971180894e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          80.0e6           Pa       temperature       670.00             K            1.503831359e-3
should_succeed   water               massic_volume                    ISTO_IAPWS_R7            pressure          80.0e6           Pa       temperature       750.00             K            1.973692940e-3
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   0.001e3          J.kg-1   massic_entropy    0.                 J.kg-1.K-1   9.800980612e-4
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   1500e3           J.kg-1   massic_entropy    3.4e3              J.kg-1.K-1   5.868294423e1
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   1700e3           J.kg-1   massic_entropy    3.8e3              J.kg-1.K-1   2.555703246e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2000e3           J.kg-1   massic_entropy    4.2e3              J.kg-1.K-1   4.540873468e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2100e3           J.kg-1   massic_entropy    4.3e3              J.kg-1.K-1   6.078123340e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2400e3           J.kg-1   massic_entropy    4.7e3              J.kg-1.K-1   6.363924887e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2600e3           J.kg-1   massic_entropy    5.1e3              J.kg-1.K-1   3.434999263e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2700e3           J.kg-1   massic_entropy    5.0e3              J.kg-1.K-1   8.839043281e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2800e3           J.kg-1   massic_entropy    5.1e3              J.kg-1.K-1   9.439202060e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2800e3           J.kg-1   massic_entropy    5.8e3              J.kg-1.K-1   8.414574124e6
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2800e3           J.kg-1   massic_entropy    6.0e3              J.kg-1.K-1   4.793911442e6
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2800e3           J.kg-1   massic_entropy    6.5e3              J.kg-1.K-1   1.371012767e6
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   2800e3           J.kg-1   massic_entropy    9.5e3              J.kg-1.K-1   1.879743844e3
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   3400e3           J.kg-1   massic_entropy    5.8e3              J.kg-1.K-1   8.376903879e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   3600e3           J.kg-1   massic_entropy    6.0e3              J.kg-1.K-1   8.395519209e7
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   3600e3           J.kg-1   massic_entropy    7.0e3              J.kg-1.K-1   7.527161441e6
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   4100e3           J.kg-1   massic_entropy    9.5e3              J.kg-1.K-1   1.024788997e5
should_succeed   water               pressure                         ISTO_IAPWS_R7            massic_enthalpy   90e3             J.kg-1   massic_entropy    0.                 J.kg-1.K-1   9.192954727e1
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       300.               K            0.427920172e3
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          0.0035e6         Pa       temperature       700.               K            0.644289068e3
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          0.5e6            Pa       temperature       1500.              K            0.917068690e3
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       300.               K            0.150773921e4
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          3e6              Pa       temperature       500.               K            0.124071337e4
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       1500.              K            0.928548002e3
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          30.e6            Pa       temperature       2000.              K            0.106736948e4
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          30e6             Pa       temperature       700.               K            0.480386523e3
should_succeed   water               speed_of_sound                   ISTO_IAPWS_R7            pressure          80e6             Pa       temperature       300.               K            0.163469054e4
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          0.001e6          Pa       massic_enthalpy   3000e3             J.kg-1       0.534433241e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          100e6            Pa       massic_enthalpy   2100e3             J.kg-1       7.336163014e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          100e6            Pa       massic_enthalpy   2700e3             J.kg-1       8.420460876e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          20e6             Pa       massic_enthalpy   1700e3             J.kg-1       6.293083892e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          20e6             Pa       massic_enthalpy   2500e3             J.kg-1       6.418418053e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          25e6             Pa       massic_enthalpy   3500e3             J.kg-1       0.875279054e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          3e6              Pa       massic_enthalpy   3000e3             J.kg-1       0.575373370e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          3e6              Pa       massic_enthalpy   4000e3             J.kg-1       0.101077577e4
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          3e6              Pa       massic_enthalpy   500e3              J.kg-1       0.391798509e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          40e6             Pa       massic_enthalpy   2700e3             J.kg-1       0.743056411e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          50e6             Pa       massic_enthalpy   2000e3             J.kg-1       6.905718338e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          50e6             Pa       massic_enthalpy   2400e3             J.kg-1       7.351848618e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          5e6              Pa       massic_enthalpy   3500e3             J.kg-1       0.801299102e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          5e6              Pa       massic_enthalpy   4000e3             J.kg-1       0.101531583e4
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          60e6             Pa       massic_enthalpy   2700e3             J.kg-1       0.791137067e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          60e6             Pa       massic_enthalpy   3200e3             J.kg-1       0.882756860e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_enthalpy   1500e3             J.kg-1       0.611041229e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_enthalpy   500e3              J.kg-1       0.378108626e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          0.1e6            Pa       massic_entropy    7.5e3              J.kg-1.K-1   0.399517097e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          0.1e6            Pa       massic_entropy    8e3                J.kg-1.K-1   0.514127081e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          100e6            Pa       massic_entropy    4.0e3              J.kg-1.K-1   7.056880237e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          100e6            Pa       massic_entropy    5.0e3              J.kg-1.K-1   8.474332825e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          2.5e6            Pa       massic_entropy    8e3                J.kg-1.K-1   0.103984917e4
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          20e6             Pa       massic_entropy    3.8e3              J.kg-1.K-1   6.282959869e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          20e6             Pa       massic_entropy    5.0e3              J.kg-1.K-1   6.401176443e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          20e6             Pa       massic_entropy    5.75e3             J.kg-1.K-1   0.697992849e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          3e6              Pa       massic_entropy    0.5e3              J.kg-1.K-1   0.307842258e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          50e6             Pa       massic_entropy    3.6e3              J.kg-1.K-1   6.297158726e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          50e6             Pa       massic_entropy    4.5e3              J.kg-1.K-1   7.163687517e2
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_entropy    0.5e3              J.kg-1.K-1   0.309979785e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_entropy    3.0e3              J.kg-1.K-1   0.565899909e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_entropy    5.25e3             J.kg-1.K-1   0.854011484e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          80e6             Pa       massic_entropy    5.75e3             J.kg-1.K-1   0.949017998e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          8e6              Pa       massic_entropy    6e3                J.kg-1.K-1   0.600484040e3
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          8e6              Pa       massic_entropy    7.5e3              J.kg-1.K-1   0.106495556e4
should_succeed   water               temperature                      ISTO_IAPWS_R7            pressure          90e6             Pa       massic_entropy    6e3                J.kg-1.K-1   0.103801126e4
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            -0.333444253966e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            -0.554468750000e-1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            -0.333444921197e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            -0.122069433940e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.209678431622e4
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.916709492200e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.159863102566e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.135714764659e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.117793449348e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           pressure          611.657          Pa       temperature       273.16             K            0.114161597779e-9
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            -0.333354873637e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            -0.918701567000e1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            -0.333465403393e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            -0.122076932550e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.209671391024e4
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.916721463419e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.159841589458e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.135705899321e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.117785291765e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           pressure          101325.          Pa       temperature       273.152519         K            0.114154442556e-9
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            -0.483491635676e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            -0.328489902347e6
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            -0.589685024936e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            -0.261195122589e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.866333195517e3
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.941678203297e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.258495528207e-4
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.291466166994e6
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.886880048115e-10
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           pressure          100e6            Pa       temperature       100.               K            0.886060982687e-10
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           -0.333444253966e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           -0.554468750000e-1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           -0.333444921197e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           -0.122069433940e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.209678431622e4
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.916709492200e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.159863102566e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.135714764659e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.117793449348e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           temperature       273.16           K        pressure          611.657            Pa           0.114161597779e-9
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           -0.333354873637e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           -0.918701567000e1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           -0.333465403393e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           -0.122076932550e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.209671391024e4
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.916721463419e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.159841589458e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.135705899321e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.117785291765e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           temperature       273.152519       K        pressure          101325.            Pa           0.114154442556e-9
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           -0.483491635676e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           -0.328489902347e6
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           -0.589685024936e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           -0.261195122589e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.866333195517e3
should_succeed   ice_Ih              density                          ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.941678203297e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.258495528207e-4
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.291466166994e6
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.886880048115e-10
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10           temperature       100.             K        pressure          100e6              Pa           0.886060982687e-10
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            611.657
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            -0.333444253966e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            -0.554468750000e-1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            -0.333444921197e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            -0.122069433940e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            0.209678431622e4
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            0.159863102566e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            0.135714764659e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            0.117793449348e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.916709492200e3   kg.m-3   temperature       273.16             K            0.114161597779e-9
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            101325.
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            -0.333354873637e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            -0.918701567000e1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            -0.333465403393e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            -0.122076932550e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            0.209671391024e4
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            0.159841589458e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            0.135705899321e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            0.117785291765e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.916721463419e3   kg.m-3   temperature       273.152519         K            0.114154442556e-9
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            100e6
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            -0.483491635676e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            -0.328489902347e6
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            -0.589685024936e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            -0.261195122589e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            0.866333195517e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            0.258495528207e-4
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            0.291466166994e6
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            0.886880048115e-10
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   density           0.941678203297e3   kg.m-3   temperature       100.               K            0.886060982687e-10
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       611.657
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       -0.333444253966e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       -0.554468750000e-1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       -0.333444921197e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       -0.122069433940e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       0.209678431622e4
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       0.159863102566e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       0.135714764659e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       0.117793449348e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       273.16           K        density           0.916709492200e3   kg.m-3       0.114161597779e-9
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       101325.
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       -0.333354873637e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       -0.918701567000e1
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       -0.333465403393e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       -0.122076932550e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       0.209671391024e4
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       0.159841589458e-3
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       0.135705899321e7
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       0.117785291765e-9
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       273.152519       K        density           0.916721463419e3   kg.m-3       0.114154442556e-9
should_succeed   ice_Ih              pressure                         ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       100e6
should_succeed   ice_Ih              massic_enthalpy                  ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       -0.483491635676e6
should_succeed   ice_Ih              massic_helmholtz_energy          ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       -0.328489902347e6
should_succeed   ice_Ih              massic_internal_energy           ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       -0.589685024936e6
should_succeed   ice_Ih              massic_entropy                   ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       -0.261195122589e4
should_succeed   ice_Ih              massic_isobaric_heat_capacity    ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       0.866333195517e3
should_succeed   ice_Ih              cubic_expansion_coefficient      ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       0.258495528207e-4
should_succeed   ice_Ih              pressure_coefficient             ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       0.291466166994e6
should_succeed   ice_Ih              isothermal_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       0.886880048115e-10
should_succeed   ice_Ih              isentropic_compressibility       ISTO_IAPWS_R10_INVERSE   temperature       100.             K        density           0.941678203297e3   kg.m-3       0.886060982687e-10
should_succeed   supercooled_water   density                          ISTO_IAPWS_G12           temperature       273.15           K        pressure          0.101325e6         Pa           999.84229
should_succeed   supercooled_water   density                          ISTO_IAPWS_G12           temperature       235.15           K        pressure          0.101325e6         Pa           968.09999
should_succeed   supercooled_water   density                          ISTO_IAPWS_G12           temperature       250              K        pressure          200e6              Pa           1090.45677
should_succeed   supercooled_water   density                          ISTO_IAPWS_G12           temperature       200              K        pressure          400e6              Pa           1185.02800
should_succeed   supercooled_water   density                          ISTO_IAPWS_G12           temperature       250              K        pressure          400e6              Pa           1151.71517
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12           temperature       273.15           K        pressure          0.101325e6         Pa           -0.683042e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12           temperature       235.15           K        pressure          0.101325e6         Pa           -29.63381e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12           temperature       250              K        pressure          200e6              Pa           3.267768e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12           temperature       200              K        pressure          400e6              Pa           6.716009e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12           temperature       250              K        pressure          400e6              Pa           4.929927e-4
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12           temperature       273.15           K        pressure          0.101325e6         Pa           5.088499e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12           temperature       235.15           K        pressure          0.101325e6         Pa           11.580785e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12           temperature       250              K        pressure          200e6              Pa           3.361311e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12           temperature       200              K        pressure          400e6              Pa           2.567237e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12           temperature       250              K        pressure          400e6              Pa           2.277029e-10
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12           temperature       273.15           K        pressure          0.101325e6         Pa           4218.3002
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12           temperature       235.15           K        pressure          0.101325e6         Pa           5997.5632
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12           temperature       250              K        pressure          200e6              Pa           3708.3902
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12           temperature       200              K        pressure          400e6              Pa           3338.5250
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12           temperature       250              K        pressure          400e6              Pa           3757.2144
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12           temperature       273.15           K        pressure          0.101325e6         Pa           1402.3886
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12           temperature       235.15           K        pressure          0.101325e6         Pa           1134.5855
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12           temperature       250              K        pressure          200e6              Pa           1668.2020
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12           temperature       200              K        pressure          400e6              Pa           1899.3294
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12           temperature       250              K        pressure          400e6              Pa           2015.8782
should_succeed   supercooled_water   pressure                         ISTO_IAPWS_G12_INVERSE   temperature       273.15           K        density           999.84229          kg.m-3       0.101325e6  
should_succeed   supercooled_water   pressure                         ISTO_IAPWS_G12_INVERSE   temperature       235.15           K        density           968.09999          kg.m-3       0.101325e6  
should_succeed   supercooled_water   pressure                         ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1090.45677         kg.m-3       200e6       
should_succeed   supercooled_water   pressure                         ISTO_IAPWS_G12_INVERSE   temperature       200              K        density           1185.02800         kg.m-3       400e6       
should_succeed   supercooled_water   pressure                         ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1151.71517         kg.m-3       400e6       
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12_INVERSE   temperature       273.15           K        density           999.84229          kg.m-3       -0.683042e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12_INVERSE   temperature       235.15           K        density           968.09999          kg.m-3       -29.63381e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1090.45677         kg.m-3       3.267768e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12_INVERSE   temperature       200              K        density           1185.02800         kg.m-3       6.716009e-4
should_succeed   supercooled_water   thermal_expansion_coefficient    ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1151.71517         kg.m-3       4.929927e-4
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12_INVERSE   temperature       273.15           K        density           999.84229          kg.m-3       5.088499e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12_INVERSE   temperature       235.15           K        density           968.09999          kg.m-3       11.580785e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1090.45677         kg.m-3       3.361311e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12_INVERSE   temperature       200              K        density           1185.02800         kg.m-3       2.567237e-10
should_succeed   supercooled_water   isothermal_compressibility       ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1151.71517         kg.m-3       2.277029e-10
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12_INVERSE   temperature       273.15           K        density           999.84229          kg.m-3       4218.3002
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12_INVERSE   temperature       235.15           K        density           968.09999          kg.m-3       5997.5632
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1090.45677         kg.m-3       3708.3902
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12_INVERSE   temperature       200              K        density           1185.02800         kg.m-3       3338.5250
should_succeed   supercooled_water   massic_isobaric_heat_capacity    ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1151.71517         kg.m-3       3757.2144
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12_INVERSE   temperature       273.15           K        density           999.84229          kg.m-3       1402.3886
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12_INVERSE   temperature       235.15           K        density           968.09999          kg.m-3       1134.5855
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1090.45677         kg.m-3       1668.2020
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12_INVERSE   temperature       200              K        density           1185.02800         kg.m-3       1899.3294
should_succeed   supercooled_water   speed_of_sound                   ISTO_IAPWS_G12_INVERSE   temperature       250              K        density           1151.71517         kg.m-3       2015.8782
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       298.15           K        density           998.               kg.m-3       889.735100e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       298.15           K        density           1200.              kg.m-3       1437.649467e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       373.15           K        density           1000.              kg.m-3       307.883622e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       433.15           K        density           1.                 kg.m-3       14.538324e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       433.15           K        density           1000.              kg.m-3       217.685358e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       873.15           K        density           1.                 kg.m-3       32.619287e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       873.15           K        density           100.               kg.m-3       35.802262e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       873.15           K        density           600.               kg.m-3       77.430195e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       1173.15          K        density           1.                 kg.m-3       44.217245e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       1173.15          K        density           100.               kg.m-3       47.640433e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       1173.15          K        density           400.               kg.m-3       64.154608e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           998.             kg.m-3   temperature       298.15             K            889.735100e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1200.            kg.m-3   temperature       298.15             K            1437.649467e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1000.            kg.m-3   temperature       373.15             K            307.883622e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1.               kg.m-3   temperature       433.15             K            14.538324e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1000.            kg.m-3   temperature       433.15             K            217.685358e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1.               kg.m-3   temperature       873.15             K            32.619287e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           100.             kg.m-3   temperature       873.15             K            35.802262e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           600.             kg.m-3   temperature       873.15             K            77.430195e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           1.               kg.m-3   temperature       1173.15            K            44.217245e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           100.             kg.m-3   temperature       1173.15            K            47.640433e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           400.             kg.m-3   temperature       1173.15            K            64.154608e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           122.               kg.m-3       25.520677e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           222.               kg.m-3       31.337589e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           272.               kg.m-3       36.228143e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           322.               kg.m-3       42.961579e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           372.               kg.m-3       45.688204e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           temperature       647.35           K        density           422.               kg.m-3       49.436256e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           122.             kg.m-3   temperature       647.35             K            25.520677e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           222.             kg.m-3   temperature       647.35             K            31.337589e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           272.             kg.m-3   temperature       647.35             K            36.228143e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           322.             kg.m-3   temperature       647.35             K            42.961579e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           372.             kg.m-3   temperature       647.35             K            45.688204e-6
#should_succeed   water               viscosity                        ISTO_IAPWS_R12           density           422.             kg.m-3   temperature       647.35             K            49.436256e-6

error_no_unit_for_this_property=1
error_unknown_substance=2
error_unkown_property=3
error_unkown_model=4
error_unkown_conditions=5
error_too_many_recursive_calls=6
error_function_not_found=7
error_missing_argument=8
error_recursive_call_failed=9
error_can_t_parse_unit=10
error_bad_unit=11
error_too_many_arguments=12

#should_fail $error_no_unit_for_this_property list_property_units '"invalid_property"'


kill -s SIGTERM $SERVER_PID
exit 0
