var teamColors = [
	0xdea200,
	0x61d33c,
	0x008bd0,
	0xe34934,
	0x00ea95,
	0x8e22ee,
	0xbbe546,
	0x3e3dff,
	0xe13bb5
],
	teamColorsSelected = [
	0xebcc6c,
	0x9ee78e,
	0x5fabf1,
	0xefb3aa,
	0x8bfdc2,
	0xbb7bf9,
	0xd8f496,
	0x8589ff,
	0xf8a5e4
];

var Team = function(game, name) {
	var self = this;

	this.name = name;
	var _bots = [];

	this.material = new THREE.MeshBasicMaterial({
			color: teamColors[game.teams.length % 9]
		});
	this.materialSelected = new THREE.MeshBasicMaterial({
		color: teamColorsSelected[game.teams.length % 9]
	});

	this.addBot = function(bot) {
		_bots.push(bot);
		bot.team = self;
	}

	this.removeBot = function(bot) {
		_bots.slice(_bots.indexOf(bot), 1);
	}

	this.getBots = function() {
		return _bots;
	}

	this.win = function() {
		// DO THE STUFF WHEN TEAM WINS
		// - MAKE BOTS JUMP
		// - BLOCK SOCKET PARSING
		// - DISPLAY '#WINNER TEAM' TEXT
	}
}

module.exports = Team;
