-- Copyright (c) 2010 - 2013, Mafia 2 Multiplayer (http://m2-multiplayer.com)
-- Coding by AaronLad <aaron@m2-multiplayer.com>

onGameInit = function(l_1_0)
	l_1_0.friendlyDoorCfg = common.ShopScripts.FriendlyDoor.Cfg
	registerForMachineMessages(l_1_0)
	DelayBuffer:Insert(StartGame, {l_1_0}, 100, 1, false)
	guid = base.ScriptMachine:getScriptGuid(l_1_0.scriptId)
	
	game.datastore:SetString("VitoA1Status", "Home")
    game.datastore:SetString("VitoA3Status", "Home")
	game.datastore:SetString("VitoB12Status", "Home")
    game.datastore:SetString("FrancescaStatus", "Home")
    game.datastore:SetString("VitoVillaStatus", "Home")
    game.datastore:SetString("DerekStatus", "Opened")
    game.datastore:SetString("BruskyStatus", "Opened")
	game.datastore:SetString("CharlieStatus", "Opened")
end

onEvent = function(l_2_0, l_2_1)
	if l_2_1:GetType() == enums.EventType.BASEMESSAGE then
		if l_2_1:GetMessage() == 1 then
			StartGame(l_2_0)
		end
	end
end

StartGame = function(l_3_0)
	CommandBuffer:Insert(l_3_0, {
		function(l_11_0) return game.sds:ActivateStreamMapLine("free_joe_load") end,
		function(l_12_0) return game.sds:ActivateStreamMapLine("free_summer_load") end,
		function(l_13_0) return game.gfx:SetWeatherTemplate("DT_RTRclear_day_noon") end,
		function(l_14_0)
			player = game.game:GetActivePlayer()
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.ENTER_VEHICLE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.ENTER_VEHICLE_DONE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.LEAVE_VEHICLE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.LEAVE_VEHICLE_DONE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.DAMAGE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.AIM)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.ON_SHOOT)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.DEATH)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.DOOR_KICK)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.DOOR_OPEN)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.HUMAN_GARAGE_ENTER)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.HUMAN_GARAGE_LEAVE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.PLAYER_AIM_ENTER)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.PLAYER_AIM_LEAVE)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.ACTION)
			player:RegisterToMessages(guid, enums.EventType.HUMAN, enums.HumanMessages.SHOT)
			player:LockControls(false)
			player:SetPos(Math:newVector(0,0,0));
			
			game.hud:Show(true)
			game.traffic:SwitchGenerators(false)
			game.traffic:SwitchFarAmbient(false)
		end	
	})
end