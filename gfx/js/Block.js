var Ressource = require('./Ressource.js');

var Block = function(map, x, y) {
	var self = this;
	var game = map.game;
	var _bots = [];

	this.x = x;
	this.y = y;
	this.map = map;
	this.mesh = null;
	this.position = {
		x: self.x - map.width / 2 + 0.5,
		y: self.y - map.height / 2 + 0.5
	};

	this.ressources = [];
	for (var i = 0; i < 7; i++)
		this.ressources[i] = new Ressource(this, x, y, i);

	this.mesh = new THREE.Mesh(new THREE.BoxGeometry(0.9, 0.2, 0.9), game.materials.block);
	this.mesh.position.set(this.position.x, -0.1, this.position.y);
	this.mesh.updateMatrix();
	this.mesh.matrixAutoUpdate = false;
	this.mesh.onclick = function(e, pick) {
		// self.mesh.position.y += 0.5;
	}
	game.scene.add(this.mesh);

	this.addBot = function(bot) {
		_bots.push(bot);
		bot.team = self;
	}

	this.removeBot = function(bot) {
		if (_bots.indexOf(bot) > -1)
			_bots.slice(_bots.indexOf(bot), 1);
	}

	this.getBots = function() {
		return _bots;
	}

	this.displayRessource = function(type) {
		for (var i in self.ressources)
			if (i == type || type == -1)
				self.ressources[i].display();
			else
				self.ressources[i].hide();
	}
}

module.exports = Block;
