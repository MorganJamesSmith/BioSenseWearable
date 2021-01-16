

// Used to prevent 3D geometries that would have zero thickness
nothing = 0.01;


wall_thickness = 3;

pcb_width = 35;
pcb_length = 57;

inner_width  = pcb_width;
inner_length = pcb_length;

outer_width = inner_width + wall_thickness * 2;
outer_length = inner_length + wall_thickness * 2;

outer_depth = 20;
inner_depth  = outer_depth  - wall_thickness * 2;

// A set of two spaced nubs that can hold a watch pin
module watch_nubs(strap_width=20, nub_size=[4, 4, 4], pin_hole_diameter=1) {
    module nub() {
        translate([strap_width/2, 0, 0])
        difference() {
            cube(nub_size);
            translate([-nothing, nub_size[0]/2, nub_size[0]/2])
                rotate(90, [0, 1, 0])
                linear_extrude(height = nub_size[0]/2 + nothing)
                circle(d = pin_hole_diameter, $fn=50);
        }
    }
    nub();
    mirror([1, 0, 0]) nub();
}

module enclosure_body(wall_thickness=3) {
    difference(){
        cube([outer_length, outer_width, outer_depth]);
        translate([wall_thickness, wall_thickness, wall_thickness*2 + nothing])
            cube([inner_length, inner_width, inner_depth]);
    }
}

translate([outer_length/2, -4, 0])
watch_nubs();
translate([outer_length/2, outer_width, 0])
watch_nubs();
enclosure_body();
