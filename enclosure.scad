

render_pcb=true;
render_enclosure=true;

// Make the heart rate sensor look more round
$fn = 100;

// Used to prevent 3D geometries that would have zero thickness
nothing = 0.01;


wall_thickness = 1.5;
bottom_thickness = 2.9;
top_thickness = 3;

pcb_size = [57, 35, 1.6];

x_overextrusion = 0.75;

outer_size = [pcb_size.x + wall_thickness * 2 + x_overextrusion * 2,
              pcb_size.y + wall_thickness * 2 + x_overextrusion * 2,
              bottom_thickness + pcb_size.z + top_thickness];

inner_offset = wall_thickness + x_overextrusion;

nub_size = [3, 3, 3];

temperature_diameter = 5.4;
temperature_height = 2.9;
temperature_position = [33.5, 17.5, -temperature_height];

// item[0] - position
// item[1] - size
// item[2] - color
heart_rate = [[24, 15.5, -1.3], [5.6, 3.3, 1.3], "hotpink"];
sd_card = [[7, 17.5, -3], [16, 17.5, 3], "silver"];
usb = [[0, 25, pcb_size.z], [7, 9, 4], "gray"];
jumpers = [[23, pcb_size.y-12, -2], [5, 12, 2], "white"];
nrf = [[pcb_size.x-16, (pcb_size.y-11)/2, -2], [16, 11, 2], "yellow"];

// cube_cutouts[0] - position
// cube_cutouts[1] - size
cube_cutouts =
    [
     // Heart Rate Sensor
     [[heart_rate[0].x, heart_rate[0].y, heart_rate[0].z - bottom_thickness],
      [heart_rate[1].x, heart_rate[1].y, heart_rate[1].z + bottom_thickness + nothing]],

     // SD Card
     [sd_card[0],
      [sd_card[1].x, sd_card[1].y + wall_thickness + nothing, sd_card[1].z + nothing]],

     // USB
     [[usb[0].x - (wall_thickness + nothing), usb[0].y, usb[0].z],
      [usb[1].x + (wall_thickness + nothing), usb[1].y, usb[1].z]],

     // Jumpers
     [jumpers[0],
      [jumpers[1].x, jumpers[1].y, jumpers[1].z + nothing]],

     // NRF
     [nrf[0],
      [nrf[1].x, nrf[1].y, nrf[1].z + nothing]],
     ];

// A set of two spaced nubs that can hold a watch pin
module watch_nubs(strap_width=20, pin_hole_diameter=1) {
    module nub() {
        translate([strap_width/2, 0, 0])
        difference() {
            cube(nub_size);
            translate([-nothing, nub_size.y/2, nub_size.z/2])
                rotate(90, [0, 1, 0])
                linear_extrude(height = nub_size.x/2 + nothing)
                circle(d = pin_hole_diameter, $fn=50);
        }
    }
    nub();
    mirror([1, 0, 0]) nub();
}

// Cutouts for components
module enclosure_cutouts() {
    for(args = cube_cutouts) {
        translate([args[0].x - x_overextrusion,
                   args[0].y - x_overextrusion,
                   args[0].z])
            cube([args[1].x + x_overextrusion*2,
                  args[1].y + x_overextrusion*2,
                  args[1].z]);
    }

    // Temperature
    translate([temperature_position.x,
               temperature_position.y,
               temperature_position.z-bottom_thickness])
        cylinder(d = temperature_diameter + x_overextrusion*2,
                 h = temperature_height+bottom_thickness+nothing);
}

module enclosure_body() {
    difference(){
        cube(outer_size);
        translate([wall_thickness, wall_thickness, bottom_thickness])
            cube([pcb_size.x+x_overextrusion*2, pcb_size.y+x_overextrusion*2, 100]);
        translate([inner_offset, inner_offset, bottom_thickness])
            enclosure_cutouts();
    }
    // TODO
    // translate([outer_size.x/2, -nub_size.y, -nub_size.z/2])
    //     watch_nubs();
    // translate([outer_size.x/2, outer_size.y, -nub_size.z/2])
    //     watch_nubs();
}

module main_board() {
    components = [heart_rate, sd_card, usb, jumpers, nrf];
    for(component = components) {
        color(component[2])
            translate(component[0])
                cube(component[1]);
    }

    // Main pcb
    color("green")
        cube(pcb_size);

    // Temperature
    color("red")
        translate(temperature_position)
            cylinder(d = temperature_diameter, h = temperature_height);
}

if(render_enclosure) {
    enclosure_body();
 }

if(render_pcb) {
    translate([inner_offset, inner_offset, bottom_thickness])
        main_board();
 }
