HeiMan = require 'HeiMan.HeiMan'

if Variable == nil then function Variable(name, value) return value end end
height = Variable('height', 1.82)
hm = Variable('hm', 3.21)

heiman = HeiMan (74, 1.74)
heiman.parameters["HeadMass"] = (hm - 1) * 2 + 1
model = heiman:create_model()
model.frames[#model.frames].body.com = {
	1, 2 * height, 3
}

return model
