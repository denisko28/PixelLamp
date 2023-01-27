/*!
*
* ColorPick jQuery plugin
* https://github.com/philzet/ColorPick.js
*
* Copyright (c) 2017-2019 Phil Zet (a.k.a. Phil Zakharchenko)
* Licensed under the MIT License
*
*/
(function( $ ) {

    $.fn.colorPick = function(config) {
        return this.each(function() {
            new $.colorPick(this, config || {});
        });
    };

    $.fn.colorPick.appendToStorage = function(hexValue) {
        this.appendToStorage(hexValue);
    };

    $.colorPick = function (element, options) {
        options = options || {};
        this.options = $.extend({}, $.fn.colorPick.defaults, options);
        if(options.str) {
            this.options.str = $.extend({}, $.fn.colorPick.defaults.str, options.str);
        }
        $.fn.colorPick.defaults = this.options;
        this.color   = this.options.initialColor.toUpperCase();
        this.element = $(element);

        var dataInitialColor = this.element.data('initialcolor');
        if (dataInitialColor) {
            this.color = dataInitialColor;
            this.appendToStorage(this.color);
        }

        var uniquePalette = [];
        $.each($.fn.colorPick.defaults.palette.map(function(x){ return x.toUpperCase() }), function(i, el){
            if($.inArray(el, uniquePalette) === -1) uniquePalette.push(el);
        });

        this.palette = uniquePalette;

        return this.element.hasClass(this.options.pickrclass) ? this : this.init();
    };

    $.fn.colorPick.defaults = {
        'initialColor': '#3498db',
        'paletteLabel': 'Default palette:',
        'allowRecent': true,
        'recentMax': 8,
        'allowCustomColor': false,
        'palette': ["#000000", "#34495E", "#8E44AD", "#C0392B", "#9B59B6", "#D35400", "#E74C3C", "#2980B9", "#16A085", "#E67E22", "#27AE60", "#3498DB", "#F39C12", "#1ABC9C", "#2ECC71", "#F1C40F", "#7F8C8D", "#95A5A6", "#DEB1AC", "#D3BEDF", "#E4BE9F", "#F6E7A7", "#ACB2B9", "#B1E2D7", "#EECCAC", "#AFB6BF", "#B0CAE0", "#BDC3C7", "#CED1D1", "#D6DBDB", "#ECF0F1", "#FFFFFF"],
        'onColorSelected': function() {}
    };

    $.colorPick.prototype = {

        init : function(){

            var self = this;
            var o = this.options;

            $.proxy($.fn.colorPick.defaults.onColorSelected, this)();

            self.show(self.element);
            $('.currColorHex').val(self.color);

            $('.currColorHex').change(function(){
                var reg=/^#([0-9a-f]{3}){1,2}$/i;
                var newColorHex = $(this).val();
                if(!reg.test(newColorHex)) {
                    newColorHex = "#000000";
                    $(this).val(newColorHex);
                }
                
                self.onColorChange(newColorHex);
                self.appendToStorage(newColorHex);
                self.updateRecent();
                $.proxy(self.options.onColorSelected, self)();
            });

            $('.colorPickButton').on("click", (event) => this.onClolorSelect(event));
            $('#recentColors').on("click", ".colorPickButton", (event) => this.onClolorSelect(event));

            return this;
        },

        onButtonClick: function(event) {
            if($(event.target).attr('hexValue') == null)
                return

            this.color = $(event.target).attr('hexValue');
            this.appendToStorage($(event.target).attr('hexValue'));
            this.updateRecent();
            $.proxy(this.options.onColorSelected, this)();
        },

        onClolorSelect: function(event) {
            this.onButtonClick(event); 
            $('.currColorHex').val(this.color);
            this.onColorChange(this.color);
        },

        onColorChange: function(newColor) {
            var colorSquare = $('.currColorSquare');
            colorSquare.css('background-color', newColor);
        },

        appendToStorage: function(color) {
	        if ($.fn.colorPick.defaults.allowRecent === true) {
	        	var storedColors = JSON.parse(localStorage.getItem("colorPickRecentItems"));
				if (storedColors == null) {
		     	    storedColors = [];
	        	}
				if ($.inArray(color, storedColors) == -1) {
		    	    storedColors.unshift(color);
					storedColors = storedColors.slice(0, $.fn.colorPick.defaults.recentMax)
					localStorage.setItem("colorPickRecentItems", JSON.stringify(storedColors));
	        	}
	        }
        },

        show: function(element) {
            $(element).append(
                '<div class="currColorPan">'+
                    '<div class="currColorSquare"></div>' + 
                    '<input class="currColorHex" type="text">' +
                '</div>'
            );

            $(element).append(
                '<div id="colorPick">'+
                    '<div id="recentColors">' + 
                        '<span>Недавние цвета:</span>' + 
                        '<div class="colorsChild" style="grid-template-columns: repeat(8, minmax(10%, 1fr))"></div>' +
                    '</div>' +
                    '<div id="staticColors">' + 
                        '<span style="margin-top:0.9rem">Стандартные цвета:</span>' + 
                        '<div class="colorsChild"></div>' + 
                    '</div>' +
                '</div>'
            );
            
            this.updateRecent();

	        jQuery.each(this.palette, function (index, item) {
				$("#staticColors > div").append('<div class="colorPickButton" hexValue="' + item + '" style="background:' + item + '"></div>');
			});
        },

        updateRecent: function() {
            $("#recentColors > div").empty();
            if (JSON.parse(localStorage.getItem("colorPickRecentItems")) == null || JSON.parse(localStorage.getItem("colorPickRecentItems")) == []) {
                $("#recentColors > div").append('<div class="colorPickButton colorPickDummy"></div>');
            } else {
                jQuery.each(JSON.parse(localStorage.getItem("colorPickRecentItems")), function (index, item) {
                    $("#recentColors > div").append('<div class="colorPickButton" hexValue="' + item + '" style="background:' + item + '"></div>');
                    if (index == $.fn.colorPick.defaults.recentMax-1) {
                        return false;
                    }
                });
            }
        }

    };

}( jQuery ));
