

function onStart()
  transform.sz = 1
  transform.sx = 1
  transform.sy = 1
end


function onUpdate(dt)
  transform.rz = transform.rz - 60 * dt
end