dofile(LockOn_Options.common_script_path.."elements_defs.lua")
dofile(LockOn_Options.script_path.."EFM_Data_Bus.lua")

-- set color values are r, g, b, a, with a min-max of 0, 255
local draw_background           = MakeMaterial("arcade.tga", {32, 96, 128, 128})
local draw_axis                 = MakeMaterial("arcade.tga", {0, 16, 48, 168})
local draw_input                = MakeMaterial("arcade.tga", {255, 192, 0, 255})
local draw_indicator            = MakeMaterial("arcade.tga", {255, 255, 255, 255})

SetCustomScale(1.0)

function AddElement(object)
	object.screenspace          = ScreenType.SCREENSPACE_TRUE
    object.use_mipfilter        = true
    Add(object)
end

local aspect                    = LockOn_Options.screen.aspect

local sizeX                     = 0.15
local sizeY                     = 0.2

local tex_scale                 = 0.25 / sizeX
local ds                        = 0.05 * sizeX
local line_width                = (4.5 / 512) / tex_scale

local throttle_shift            = 0.3 * sizeX
local rud_shift                 = 0.1 * sizeX

-- draw BACKGROUND
base       			            = CreateElement "ceMeshPoly"
base.name		                = "base"
base.primitivetype              = "triangles"
base.material                   = draw_background
base.vertices                   = {
                                    {-(sizeX + throttle_shift + rud_shift + 3.0 * line_width + ds), -sizeY},
                                    {-(sizeX + throttle_shift + rud_shift + 3.0 * line_width + ds), sizeY}, 
                                    { sizeX + ds, sizeY},
                                    { sizeX + ds, -sizeY}
                                  }        
base.indices                    = default_box_indices
base.init_pos                   = {(-1 * aspect + 1.5 * sizeX),(1 - 1.3 * sizeX)}
base.element_params             = {"SHOW_CONTROLS"} 
base.controllers                = {{"parameter_in_range", 0, 1}}
base.h_clip_relation            = h_clip_relations.REWRITE_LEVEL
base.level		                = 8
AddElement(base)

-- scale PITCH
pitch_scale                     = CreateElement "ceTexPoly"
pitch_scale.name		        = "pitch_scale"
pitch_scale.vertices            = {
                                    {-sizeX,-line_width},
                                    {-sizeX, line_width},
                                    {sizeX, line_width},
                                    {sizeX, -line_width}
                                  }
pitch_scale.indices		        = default_box_indices
pitch_scale.material	        = draw_axis
pitch_scale.init_rot            = {90, 0, 0}
pitch_scale.tex_params	        = {256 / 512, 176.5 / 512, 0.5 * tex_scale, 2 * tex_scale}
pitch_scale.parent_element      = base.name
AddElement(pitch_scale)

-- scale ROLL
roll_scale                      = CreateElement "ceTexPoly"
roll_scale.name		            = "roll_scale"
roll_scale.vertices             = {
                                    {-sizeX,-line_width},
                                    {-sizeX, line_width},
                                    {sizeX, line_width},
                                    {sizeX,-line_width}
                                  }
roll_scale.indices		        = default_box_indices
roll_scale.material	            = draw_axis
roll_scale.tex_params	        = {256 / 512, 176.5 / 512, tex_scale, 2 * tex_scale}
roll_scale.parent_element       = base.name
AddElement(roll_scale)

local stick_index_sz            = 0.1 * sizeX

-- draw PITCH AND ROLL position
stick_position					= CreateElement "ceTexPoly"
stick_position.name             = "stick_position"
stick_position.vertices         = {
                                    {-stick_index_sz, -stick_index_sz},
                                    {-stick_index_sz, stick_index_sz},
                                    {stick_index_sz, stick_index_sz},
                                    {stick_index_sz, -stick_index_sz}
                                  }
stick_position.indices          = default_box_indices
stick_position.material	        = draw_input
stick_position.tex_params	    = {330 / 512, 365.5 / 512, 2 * tex_scale, 2 * tex_scale / 0.8}
stick_position.element_params   = {"STICK_PITCH", "STICK_ROLL"} 
stick_position.controllers      = {
                                    {"move_left_right_using_parameter", 1, sizeX},
								    {"move_up_down_using_parameter", 0, sizeX}
                                  }
stick_position.parent_element   = base.name
AddElement(stick_position)

-- scale RUDDER
rudder_scale                    = Copy(roll_scale)
rudder_scale.init_pos           = {0,-(sizeX + rud_shift)}
AddElement(rudder_scale)

-- draw RUDDER position
rudder_index                    = Copy(roll_scale)
rudder_index.vertices           = {
                                    {-2.0 * line_width, -line_width},
                                    {-2.0 * line_width, line_width},
                                    {2.0 * line_width, line_width},
                                    {2.0 * line_width, -line_width}
                                  }
rudder_index.element_params     = {"RUDDER_PEDALS"}  
rudder_index.controllers 	    = {{"move_up_down_using_parameter", 0, -sizeX}}
rudder_index.init_rot           = {90, 0}
rudder_index.parent_element     = rudder_scale.name
rudder_index.tex_params	        = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
rudder_index.material	        = draw_input
AddElement(rudder_index)

-- scale THROTTLE position
throttle_scale                  = Copy(pitch_scale)
throttle_scale.vertices         = {
                                    {0, -line_width},
                                    {0, line_width},
                                    {2.0 * sizeX, line_width},
                                    {2.0 * sizeX, -line_width}
                                  }
throttle_scale.init_pos         = {-(sizeX + throttle_shift), -sizeX}
AddElement(throttle_scale)

-- draw THROTTLE position
throttle_index                  = Copy(roll_scale)
throttle_index.vertices         = {
                                    {-2.0 * line_width, -line_width},
                                    {-2.0 * line_width, line_width},
                                    { 2.0 * line_width, line_width},
                                    { 2.0 * line_width, -line_width}
                                  }
throttle_index.element_params   = {"FM_THROTTLE_POSITION"}  
throttle_index.controllers      = {{"move_up_down_using_parameter", 0, 2.0 * sizeX}}
throttle_index.tex_params	    = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
throttle_index.init_rot         = {-90, 0, 0}
throttle_index.material         = draw_input
throttle_index.parent_element   = throttle_scale.name
AddElement(throttle_index)

-- scale WHEELBRAKE LEFT position
wbrakel_scale                = Copy(pitch_scale)
wbrakel_scale.vertices       = {
                                    {0, -line_width},
                                    {0, line_width},
                                    {sizeX - 8.0 * ds, line_width},
                                    {sizeX - 8.0 * ds, -line_width}
                                  }
wbrakel_scale.init_pos       = {-sizeX + 2.0 * ds, -sizeX}
AddElement(wbrakel_scale)

-- draw WHEELBRAKE LEFT position
wbrakel_index                = Copy(roll_scale)
wbrakel_index.vertices       = {
                                    {-2.0 * line_width, -line_width},
                                    {-2.0 * line_width, line_width},
                                    { 2.0 * line_width, line_width},
                                    { 2.0 * line_width, -line_width}
                                  }
wbrakel_index.element_params = {"LEFT_BRAKE_PEDAL"}  
wbrakel_index.controllers    = {{"move_up_down_using_parameter", 0, sizeX - 8.0 * ds}}
wbrakel_index.tex_params	    = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
wbrakel_index.init_rot         = {-90, 0, 0}
wbrakel_index.material         = draw_input
wbrakel_index.parent_element   = wbrakel_scale.name
AddElement(wbrakel_index)

-- scale WHEELBRAKE RIGHT position
wbraker_scale                = Copy(pitch_scale)
wbraker_scale.vertices       = {
                                    {0, -line_width},
                                    {0, line_width},
                                    {sizeX - 8.0 * ds, line_width},
                                    {sizeX - 8.0 * ds, -line_width}
                                  }
wbraker_scale.init_pos       = {sizeX - 2.0 * ds, -sizeX}
AddElement(wbraker_scale)

-- draw WHEELBRAKE RIGHT position
wbraker_index                = Copy(roll_scale)
wbraker_index.vertices       = {
                                    {-2.0 * line_width, -line_width},
                                    {-2.0 * line_width, line_width},
                                    { 2.0 * line_width, line_width},
                                    { 2.0 * line_width, -line_width}
                                  }
wbraker_index.element_params = {"RIGHT_BRAKE_PEDAL"}  
wbraker_index.controllers    = {{"move_up_down_using_parameter", 0, sizeX - 8.0 * ds}}
wbraker_index.tex_params	    = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
wbraker_index.init_rot         = {-90, 0, 0}
wbraker_index.material         = draw_input
wbraker_index.parent_element   = wbraker_scale.name
AddElement(wbraker_index)


-- scale FLAPS position
flaps_scale                = Copy(pitch_scale)
flaps_scale.vertices       = {
                                {0, -line_width},
                                {0, line_width},
                                {-sizeX + 14.0 * ds, line_width},
                                {-sizeX + 14.0 * ds, -line_width}
                              }
flaps_scale.init_pos       = {-sizeX + 2.0 * ds, 0}
AddElement(flaps_scale)

-- draw FLAPS position
flaps_index                 = Copy(roll_scale)
flaps_index.vertices        = {
                                {-2.0 * line_width, -line_width},
                                {-2.0 * line_width, line_width},
                                { 2.0 * line_width, line_width},
                                { 2.0 * line_width, -line_width}
                              }
flaps_index.element_params  = {"FM_FLAPS"}  
flaps_index.controllers     = {{"move_up_down_using_parameter", 0, -sizeX + 14.0 * ds}}
flaps_index.tex_params	    = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
flaps_index.init_rot        = {-90, 0, 0}
flaps_index.material        = draw_indicator
flaps_index.parent_element  = flaps_scale.name
AddElement(flaps_index)

-- scale SPOILER position
spoiler_scale                = Copy(pitch_scale)
spoiler_scale.vertices       = {
                                {0, -line_width},
                                {0, line_width},
                                {sizeX - 14.0 * ds, line_width},
                                {sizeX - 14.0 * ds, -line_width}
                              }
spoiler_scale.init_pos       = {-sizeX + 2.0 * ds, 0}
AddElement(spoiler_scale)

-- draw SPOILER position
spoiler_index                 = Copy(roll_scale)
spoiler_index.vertices        = {
                                {-2.0 * line_width, -line_width},
                                {-2.0 * line_width, line_width},
                                { 2.0 * line_width, line_width},
                                { 2.0 * line_width, -line_width}
                              }
spoiler_index.element_params  = {"FM_SPOILERS"}  
spoiler_index.controllers     = {{"move_up_down_using_parameter", 0, sizeX - 14.0 * ds}}
spoiler_index.tex_params	    = {256 / 512, 176.5 / 512, 0.5 * tex_scale / 3, 2 * tex_scale / 3}
spoiler_index.init_rot        = {-90, 0, 0}
spoiler_index.material        = draw_indicator
spoiler_index.parent_element  = spoiler_scale.name
AddElement(spoiler_index)
