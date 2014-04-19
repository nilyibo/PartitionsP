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

function processResponse(response) {
	var result = document.getElementById('result-div');
	result.innerHTML = "";

	for (var i = 0; i < response.length - 1; ++i)	// Last one is null
	{
		result.innerHTML += response[i].parity + '<br>';
	}
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
	processResponse(response);

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