Blockly.JavaScript['cmmcRemote_setup'] = function (block) {
	var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
	var value_password = Blockly.JavaScript.valueToCode(block, 'PSK', Blockly.JavaScript.ORDER_ATOMIC);

	var code = `
#EXTINC#include <cmmcReceive.h>#END
#VARIABLEcmmcReceive remote;#END
remote.begin(${value_name}, ${value_password});
\n`;

	return code;
};

Blockly.JavaScript["cmmcRemote_loop"] = function(block) {
	var statements_mqtt_statement = Blockly.JavaScript.statementToCode(block,"LOOP_STATEMENT");

	var code = `
remote.loop();
if(remote.getState()) {
${statements_mqtt_statement}
}
\n`;
	return code;
  };

Blockly.JavaScript['cmmcRemote_getRoll'] = function (block) {
	var code = `remote.getRoll()`;
	return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['cmmcRemote_getPitch'] = function (block) {
	var code = `remote.getPitch()`;
	return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['cmmcRemote_getThrottle'] = function (block) {
	var code = `remote.getThrottle()`;
	return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['cmmcRemote_getYaw'] = function (block) {
	var code = `remote.getYaw()`;
	return [code, Blockly.JavaScript.ORDER_NONE];
};