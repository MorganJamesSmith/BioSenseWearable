

// Used to prevent 3D geometries that would have zero thickness
nothing = 0.01;


wall_thickness = 2;
bottom_thickness = 3.7;

pcb_size = [57, 35, 1.6];

outer_size = [pcb_size.x + wall_thickness * 2,
              pcb_size.y + wall_thickness * 2,
              10];

nub_size = [3, 3, 3];


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

module enclosure_body() {
    difference(){
        cube(outer_size);
        translate([wall_thickness, wall_thickness, bottom_thickness])
            cube([pcb_size.x, pcb_size.y, 100]);
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
    // TODO: get actual size data
    color("grey")
        translate([7, 17.5, -2])
        cube([16, 17.5, 2]);

    // USB
    // TODO: get actual size data
    color("grey")
        translate([0, 20, pcb_size.z])
        cube([7.5, 14, 2]);

    // Heart rate
    color("blue")
        translate([24, 15.5, -1.3])
        cube([5.6, 3.3, 1.3]);

    // Temperature
    color("red")
        translate([33.5, 17.5, -2.9])
        cylinder(d = 5.4, h = 2.9);
    }
}

enclosure_body();
translate([wall_thickness, wall_thickness, bottom_thickness])
main_board();
