HeiMan = require 'HeiMan.HeiMan'

if Variable == nil then function Variable(name, value) return value end end
height = Variable('height', 1.82)
weight = Variable('weight', 74)

heiman = HeiMan (weight, 1.74)
model = heiman:create_model()
model.frames[#model.frames].body.com = {
	1, 2 * height, 3
}
model.frames[#model.frames].visuals[1].mesh_center = {
	0, height, 0.09143245
}

return model
