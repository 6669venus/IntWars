function finishCasting()
   print("Speed increase" ..getEffectValue(3))
   local buff = Buff.new("RaiseMorale", 7.0, getOwner())
   buff:setMovementSpeedPercentModifier(getEffectValue(3))
   addBuff(buff)
end

function applyEffects()

end
