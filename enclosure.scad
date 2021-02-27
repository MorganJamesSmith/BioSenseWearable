
// Make the heart rate sensor look more round
$fn = 100;

// Used to prevent 3D geometries that would have zero thickness
nothing = 0.01;


wall_thickness = 2;
bottom_thickness = 3.7;
top_thickness = 3;

pcb_size = [57, 35, 1.6];

outer_size = [pcb_size.x + wall_thickness * 2,
              pcb_size.y + wall_thickness * 2,
              bottom_thickness + pcb_size.z + top_thickness];

nub_size = [3, 3, 3];

heart_rate_size = [5.6, 3.3, 1.3];
heart_rate_position = [24, 15.5, -heart_rate_size.z];

temperature_diameter = 5.4;
temperature_height = 2.9;
temperature_position = [33.5, 17.5, -temperature_height];

// TODO: get actual size data
sd_card_size = [16, 17.5, 2];
sd_card_position = [7, 17.5, -sd_card_size.z];

// TODO: get actual size data
usb_size = [7.5, 14, 2];
usb_position = [0, 20, pcb_size.z];

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

// Cutouts used for the heart rate sensor, temperature sensor, USB, and SD card
module enclosure_cutouts() {
    // SD Card
    translate(sd_card_position)
        cube([sd_card_size.x, sd_card_size.y+wall_thickness+nothing, sd_card_size.z]);

    // USB
    translate([usb_position.x-wall_thickness-nothing, usb_position.y, usb_position.z])
        cube([usb_size.x+wall_thickness+nothing, usb_size.y, usb_size.z]);

    // Heart rate
    translate([heart_rate_position.x,
               heart_rate_position.y,
               heart_rate_position.z-bottom_thickness])
        cube([heart_rate_size.x, heart_rate_size.y, heart_rate_size.z+bottom_thickness]);

    // Temperature
    translate([temperature_position.x,
               temperature_position.y,
               temperature_position.z-bottom_thickness])
        cylinder(d = temperature_diameter, h = temperature_height+bottom_thickness);
}

module enclosure_body() {
    difference(){
        cube(outer_size);
        translate([wall_thickness, wall_thickness, bottom_thickness])
            cube([pcb_size.x, pcb_size.y, 100]);
        translate([wall_thickness, wall_thickness, bottom_thickness])
            enclosure_cutouts();
    }
    translate([outer_size.x/2, -nub_size.y, -nub_size.z/2])
        watch_nubs();
    translate([outer_size.x/2, outer_size.y, -nub_size.z/2])
        watch_nubs();
}

module main_board() {
    union() {
    // Main pcb
    color("green")
        cube(pcb_size);

    // SD Card
    color("grey")
        translate(sd_card_position)
            cube(sd_card_size);

    // USB
    color("grey")
        translate(usb_position)
            cube(usb_size);

    // Heart rate
    color("blue")
        translate(heart_rate_position)
            cube(heart_rate_size);

    // Temperature
    color("red")
        translate(temperature_position)
            cylinder(d = temperature_diameter, h = temperature_height);
    }
}

enclosure_body();
translate([wall_thickness, wall_thickness, bottom_thickness])
    main_board();
