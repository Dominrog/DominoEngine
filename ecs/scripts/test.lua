

function onStart()
  transform.sz = 1
  transform.sx = 1
  transform.sy = 1
end


function onUpdate(dt)
  transform.z = transform.z - 0.5 * dt
  transform.rz = transform.rz - 60 * dt
end