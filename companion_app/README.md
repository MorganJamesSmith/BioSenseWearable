
layers:
5. read csv and generate plot
4. adjust timestamps and units to correspond to real units instead of direct measurements
3. parse payloads and split by type
2. Parse entry headers, list of entries
1. connect to device over bluetooth and extract log data

each of these layers is pretty straight forward and seperates the task into independent chuncks so that they could be developed independently. (the order of development was 2,3,5,1,4)

`device_comms.py` deals with layer 1 to extract data over bluetooth
`log_parser.py` deals with layers 2 and 3
layer 4 hasn't been written at time of writing this readme
`ploting.py` deals with layer 5

additionally `struct_types.py` contains some relevent code for parsing the struct format easily.

- why it is companion app important ( in order to extract data )
- bluetooth is used because it is ubiquitus
- go into more detail for what the graph is actually showing (units are not done yet)
- moving forward units are not done yet,

# TEST PLAN

- bluetooth
- CLI
    - read in smaller chunks
    - detect "failed to open file"
    - when first character goes missing figure out better solution
    - fails first time sometimes because of weird symbol that is sent from nowhere
- headers
    - test that timestamps don't go down by more than a few milliseconds
    - ensure first command is reset - is hard because only applicable when starting from reset
    - test behaviour when file is corrupted (wrong size)
        - last entry in file is incorrect size
        - entry right before reset is invalid
- payloads
    - test invalid entry types
    - invalid entry length
- units
    - make unit conversion code
- plotting

- checking that offsets actually line up with all the resets