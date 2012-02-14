function documentReady()
{
  $("#tabs").tabs();
  setInterval('updateInfo()', 1000);
  setInterval('updateDiskData()', 1000);

  $('#ui-log-tab').bind('click', function(){ 
    updateLogs();
  });

}

function updateLogs()
{
  $.ajax({
    url: '/logs',
    context: document.body,
    success: function(data)
    {
      $('#log-tab').html(data.replace(/\n/g, '<br/>'));
    }
  });
}

function updateInfo()
{
  $.ajax({
    url: '/dwipe.json',
    context: document.body,
    success: function(data)
    {
      data = JSON.parse(data);
      $("#entropy").html(data.info.entropy); 
      $("#prng").html(data.info.prng); 
      $("#method").html(data.info.method); 
      $("#verify").html(convertVerify(data.info.verify)); 
      $("#rounds").html(data.info.rounds); 
      $("#runtime").html(data.info.runtime ? data.info.runtime : "N/A"); 
      $("#remaining").html(data.info.remaining ? data.info.remaining : "N/A"); 
      $("#loadavg").html(data.info.load_avg ? data.info.load_avg : "N/A"); 
      $("#throughput").html(convertKiloBytes(data.info.throughput)); 
      $("#errors").html(data.info.errors); 

      $("#all-count").html(data.info.total_disks); 
      $("#wiping-count").html(data.info.wiping_disks); 
    }
  });
}

function updateDiskData()
{
  var selected = $('#tabs').tabs('option', 'selected');
  if (selected > 1)
  {
    return;
  }

  var theUrl = selected ? '/disks/all.json' : '/disks/wiping.json';

  $.ajax({
    url: theUrl,
    context: document.body,
    success: function(data)
    {
      data = JSON.parse(data);

      if ($(data.disks).length > 0)
      {
        $('#' + (selected ? 'all' : 'wiping') + '-no-data').remove();
      }

      $(data.disks).each(function(index, value)
      {
        disk = value;
        var dataTable = new google.visualization.DataTable();
        dataTable.addColumn('string', 'Wiping');
        dataTable.addColumn('number', 'Percentage');
        dataTable.addRows([
          ['Current pass', Math.round(disk.round.percent)],
          ['Not nuked', Math.round(100 - disk.round.percent)],
        ]);
        var options = {
                       'width':200,
                       'height':200,
                       'is3D': true,
                       'slices': { 0: { color: 'grey' }, 1: { color: 'green' } },
                       'chartArea': { top: 0, left: "30%" },
                       'legend': {position: 'none'},
                       'backgroundColor': { fill: 'transparent'},
                       'tooltip.text': 'percentage'};

       if (!document.getElementById(selected + '_chart_' + disk.name))
       {
         var element = generateDiskElement(selected, disk.name);
         element.appendTo(selected ? "#all-tab" : "#wiping-tab");
       }

      document.getElementById(selected + '_size_' + disk.name).innerHTML = convertBytes(disk.device.size);
      document.getElementById(selected + '_throughput_' + disk.name).innerHTML = convertBytes(disk.throughput) + '/s';
      document.getElementById(selected + '_verify_errors_' + disk.name).innerHTML = disk.verify_errors;
      document.getElementById(selected + '_pass_' + disk.name).innerHTML = disk.pass.working + '/' + disk.pass.count;
      document.getElementById(selected + '_round_' + disk.name).innerHTML = disk.round.working + '/' + disk.round.count;
      var chart = new google.visualization.PieChart(document.getElementById(selected + '_chart_' + disk.name));
      chart.draw(dataTable, options);
    });
  }});
}

function generateDiskElement(selected, name)
{
  return  $('<div class="disk-info ui-widget ui-widget-content ui-corner-all">' +
          '<h3>' + name + '</h3>' +
          '<ul>' +
          '  <li>Size : <span id="' + selected + '_size_' + name + '"></span></li>' +
          '  <li>Throughput : <span id="' + selected + '_throughput_' + name + '"></span></li>' +
          '  <li>Verify Errors : <span id="' + selected + '_verify_errors_' + name + '"></span></li>' +
          '</ul>' +
          '<div id="' + selected + '_chart_' + name + '"></div>' +
          '<div class="disk-footer">' +
          'Pass <span id="' + selected + '_pass_' + name + '"></span> | ' +
          'Round <span id="' + selected + '_round_' + name + '"></span>' +
          '</div>' +
          '</div>');
}

function convertKiloBytes(value)
{
  if (value > 1024)
  {
    return Math.round(value / 1024) + " MB/s"; 
  }

  return value + " KB/s";
}

function convertBytes(value)
{
  if (value > 1024 * 1024 * 1024)
  {
    return Math.round(value / (1024 * 1024 * 1024)) + " GB"; 
  }
  else if (value > 1024 * 1024)
  {
    return Math.round(value / (1024 * 1024)) + " MB"; 
  }
  else if (value > 1024)
  {
    return value + " KB";
  }

  return value + " B";
}

function convertVerify(value)
{
  switch (value)
  {
    case 0: return "None";
    case 1: return "Last step";
    case 2: return "All steps"; 
  }
}
