/**
 * This file contains the function for PartitionsP data
 *
 * @file default.js
 * @author Yibo G (https://github.com/nilyibo)
 */
var params = [];	// l, nmin, nmax

/**
 * UI handlers
 */

function requestButton_click() {
	// Disable buttons
	toggleUIInputs(false);

	params = inputParams();
	if (params.length == 0)
	{
		toggleUIInputs(true);
		var result = document.getElementById('result-div');
		result.innerHTML = "Please correct the error(s).";
		return;
	}

	sendQuery();
}

function inputKeyDown(event) {
	if (event.keyCode == 13)	// 'Enter' key
		document.getElementById('requestButton').click();
}

// If enabled, then turn on inputs; otherwise, disable them
function toggleUIInputs(enabled) {
	var requestButton = document.getElementById('requestButton');
	var lSelect = document.getElementById("lSelect");
	var nminInput = document.getElementById("nminInput");
	var nmaxInput = document.getElementById("nmaxInput");
	if (enabled)
	{
		requestButton.disabled = false;
		lSelect.disabled = false;
		nminInput.disabled = false;
		nmaxInput.disabled = false;
	}
	else
	{
		requestButton.disabled = true;
		lSelect.disabled = true;
		nminInput.disabled = true;
		nmaxInput.disabled = true;
	}
}

/**
 * Data Transfer helper function
 */

function inputParams() {
	var lSelect = document.getElementById("lSelect");
	var nminInput = document.getElementById("nminInput");
	var nmaxInput = document.getElementById("nmaxInput");

	var l = parseInt(lSelect.options[lSelect.selectedIndex].value);
	var nmin = parseInt(nminInput.value);
	var nmax = parseInt(nmaxInput.value);

	if (isNaN(nmin) || nmin < 0)
	{
		inputWarning('nminInput');
		return [];
	}

	if (isNaN(nmax) || nmax < nmin)
	{
		inputWarning('nmaxInput');
		return [];
	}

	var status = document.getElementById('status-div');
	status.innerHTML = 'Waiting for response.';
	status.style.color = '#000000';
	return [l, nmin, nmax];
}

function inputWarning(id) {
	var status = document.getElementById('status-div');
	status.innerHTML = 'Invalid parameter.';
	status.style.color = '#ff0000';

	var input = document.getElementById(id);
	input.select();
}

// Returns 0 on success, -1 on error
function processResponse(response) {
	var result = document.getElementById('result-div');
	result.innerHTML = ("<i>l</i>=" + params[0] + ", <i>n</i> from "
						+ params[1] + "  to " + params[2] + "(100 per line)<br>");

	var start = params[1], end = params[2];
	var SIZE = 32;
	var alignedStart = start - (start % SIZE);
	var alignedEnd = end - (end % SIZE);

	if (alignedStart > alignedEnd)
		return -1;
	else if (alignedStart == alignedEnd)
	{
		if (response.length != 2)
			return -1;
		var n = parseInt(response[0].n);
		if (n != alignedStart)
			return -1;
		for (var i = start; i <= end; ++i)
		{
			var parity = parseInt(response[0].parity);
			if (isNaN(parity))
				return -1;
			result.innerHTML += ((parity >> (i - alignedStart)) & 0x1);
		}
	}
	else
	{
		var counter = 0;

		// Validate date
		if (alignedEnd - alignedStart != (response.length - 2) * SIZE)
			return -1;
		for (var i = 0; i < response.length - 1; ++i)
		{
			var n = parseInt(response[i].n);
			if (n != alignedStart + SIZE * i)
				return -1;
		}

		// Data output
		for (var i = start; i < alignedStart + SIZE; ++i)
		{
			var parity = parseInt(response[0].parity);
			result.innerHTML += ((parity >> (i % SIZE)) & 0x1);
			++counter;
		}

		for (var i = alignedStart + SIZE; i < alignedEnd; ++i)
		{
			var index = Math.floor((i - alignedStart) / SIZE);
			var parity = parseInt(response[index].parity);
			result.innerHTML += ((parity >> (i % SIZE)) & 0x1);
			++counter;
			if (counter % 100 == 0)
			{
				counter = 0;
				result.innerHTML += '<br>';
			}
		}

		for (var i = alignedEnd; i <= end; ++i)
		{
			var parity = parseInt(response[response.length - 2].parity);
			result.innerHTML += ((parity >> (i % SIZE)) & 0x1);
			++counter;
			if (counter % 100 == 0)
			{
				counter = 0;
				result.innerHTML += '<br>';
			}
		}
	}

	return 0;
}

/**
 * PHP related query functions
 */

// sends the query
function sendQuery() {
	// Check for n range
	var tooLarge = (params[2] - params[1]) >= 5000;
	if(tooLarge && !confirm("n range is very large.\n"
			+ "This may cause the page to be unresponsive for some time.\n"
			+ "Do you want to continue?")) {
		toggleUIInputs(true);
		return false;
	}

	getRequest(
		'./query.php',	// URL for the PHP file
		querySuccess,	// handle successful request
		queryError		// handle error
	);
	return false;
}

// handles successful response
function querySuccess(queryStatus, responseText) {
	var status = document.getElementById('status-div');
	if (queryStatus == 200)
		status.innerHTML = "Status: OK";
	else
		status.innerHTML = "Status: " + queryStatus;
	status.style.color = '#000000';

	if (responseText == 'Error')
	{
		var result = document.getElementById('result-div');
		result.innerHTML = "Data not found.";
		toggleUIInputs(true);
		return;
	}

	var response = JSON.parse(responseText);
	if (processResponse(response) == -1)
	{
		var result = document.getElementById('result-div');
		result.innerHTML = "Parse error. Server returned unrecognized data.";
	}

	// Restore buttons
	toggleUIInputs(true);
}

// handles error
function queryError(queryStatus) {
	var status = document.getElementById('status-div');
	status.innerHTML = "Error: " + queryStatus;
	status.style.color = '#ff0000';

	var result = document.getElementById('result-div');
	result.innerHTML = "Connection error. Please try again later.";

	// Restore buttons
	toggleUIInputs(true);
}

// helper function for cross-browser request object
// and passing parameters
function getRequest(url, success, error) {
	// Prepare request
	var req = false;
	try{
		// most browsers
		req = new XMLHttpRequest();
	} catch (e){
		// IE
		try{
			req = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			// try an older version
			try{
				req = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e){
				return false;
			}
		}
	}

	// Error checking
	if (!req) return false;
	if (typeof success != 'function') success = function () {};
	if (typeof error!= 'function') error = function () {};

	// Register got-response handler
	req.onreadystatechange = function(){
		if(req .readyState == 4){
			return req.status === 200 ? 
				success(req.status, req.responseText) : error(req.status)
			;
		}
	}

	var urlAndParam = url + "?l=" + params[0]
					+ "&nmin=" + params[1] + "&nmax=" + params[2];	// TODO: change this

	// Send out request
	req.open("GET", urlAndParam, true);
	req.send(null);
	return req;
}