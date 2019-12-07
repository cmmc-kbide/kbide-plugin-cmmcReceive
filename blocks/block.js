Blockly.Blocks['cmmcRemote_setup'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : Setup board name");
		this.appendValueInput("NAME")
			.setCheck("String");
		this.setInputsInline(true);
		this.setPreviousStatement(true, null);
		this.setNextStatement(true, null);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

Blockly.Blocks["cmmcRemote_loop"] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : Recieve loop")
		this.appendStatementInput("LOOP_STATEMENT")
			.setCheck(null);
		this.setInputsInline(true);
		this.setPreviousStatement(true, null);
		this.setNextStatement(true, null);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

Blockly.Blocks['cmmcRemote_reset'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : Reset board");
		this.setInputsInline(true);
		this.setPreviousStatement(true, null);
		this.setNextStatement(true, null);
		this.setColour(180);
		this.setTooltip("reset board");
		this.setHelpUrl("");
	}
};

Blockly.Blocks['cmmcRemote_getRoll'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : getRoll")
		this.setOutput(true, ["Number"]);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

Blockly.Blocks['cmmcRemote_getPitch'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : getPitch")
		this.setOutput(true, ["Number"]);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

Blockly.Blocks['cmmcRemote_getThrottle'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : getThrottle")
		this.setOutput(true, ["Number"]);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

Blockly.Blocks['cmmcRemote_getYaw'] = {
	init: function () {
		this.appendDummyInput()
			.appendField("cmmcRemote : getYaw")
		this.setOutput(true, ["Number"]);
		this.setColour(180);
		this.setTooltip("");
		this.setHelpUrl("");
	}
};

