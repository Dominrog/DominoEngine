

local function getForward()
  local yaw   = math.rad(transform.ry)
  local pitch = math.rad(transform.rx)

  local x = math.cos(pitch) * math.sin(yaw)
  local y = math.sin(pitch)
  local z = math.cos(pitch) * math.cos(yaw)

  return x, y, z
end

local function getRight()
  local yaw = math.rad(transform.ry)
  return math.cos(yaw), 0, -math.sin(yaw)
end


function onStart()
  transform.sz = 1
  transform.sx = 1
  transform.sy = 1
end


function onUpdate(dt)
  local dx = Input.mouseDX()
  local dy = Input.mouseDY()
  local velocity = 5

  -- rotate object
  transform.ry = transform.ry - dx * dt * 25
  transform.rx = transform.rx - dy * dt * 25

  local fx, fy, fz = getForward()
  local rx, ry, rz = getRight()

  if Input.isDown("W") then
    transform.x = transform.x + fx * velocity * dt
    transform.y = transform.y - fy * velocity * dt
    transform.z = transform.z + fz * velocity * dt
  end

  if Input.isDown("S") then
    transform.x = transform.x - fx * velocity * dt
    transform.y = transform.y + fy * velocity * dt
    transform.z = transform.z - fz * velocity * dt
  end

  if Input.isDown("A") then
    transform.x = transform.x + rx * velocity * dt
    transform.y = transform.y + ry * velocity * dt
    transform.z = transform.z + rz * velocity * dt
  end

  if Input.isDown("D") then
    transform.x = transform.x - rx * velocity * dt
    transform.y = transform.y - ry * velocity * dt
    transform.z = transform.z - rz * velocity * dt
  end
end
