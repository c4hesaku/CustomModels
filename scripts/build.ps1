Param(
    [Parameter(Mandatory=$false)]
    [Switch] $clean,

    [Parameter(Mandatory=$false)]
    [Switch] $hotReload,

    [Parameter(Mandatory=$false)]
    [Switch] $help
)

if ($help -eq $true) {
    Write-Output "`"Build`" - Copiles your mod into a `".so`" or a `".a`" library"
    Write-Output "`n-- Arguments --`n"

    Write-Output "-HotReload `t Enables BSML hot reload for UI components"
    Write-Output "-Clean `t`t Deletes the `"build`" folder, so that the entire library is rebuilt"

    exit
}

if ($clean.IsPresent -and (Test-Path -Path "build")) {
    Remove-Item build -R
}

if (-not (Test-Path -Path "build")) {
    New-Item -Path build -ItemType Directory
}

$def = "OFF"
if ($hotReload.IsPresent) {
    $def = "ON"
}

& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DHOT_RELOAD="$def" -B build
& cmake --build ./build
