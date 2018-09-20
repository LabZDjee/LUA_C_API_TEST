
--[[
    Simple Lua test calling a C function
--]]
function process(verb, object)
 -- called C function returns a variable number of values
 -- so we cannot get them simply like this: x, y, z = getValueFromController(..)
 -- using the {} operator turns them into an array
 local values = {getValueFromController(verb.."/"..object)} -- ..: concatenation
 local sum = 0
 for _, element in ipairs(values) do -- underscore: don't care
  sum = sum + element -- type coercion: sum is num, so element turned into num
 end
 return sum, sum / #values -- #: last (one-based) index actually
end