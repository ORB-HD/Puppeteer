HeiMan = require 'HeiMan.HeiMan'

if Variable == nil then function Variable(name, value) return value end end
height = Variable('height', 1.82)
weight = Variable('weight', 74)

heiman = HeiMan (weight, height)
model = heiman:create_model()

return model
