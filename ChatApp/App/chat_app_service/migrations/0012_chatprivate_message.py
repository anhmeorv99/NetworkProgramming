# Generated by Django 3.1.3 on 2021-01-10 15:05

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('chat_app_service', '0011_auto_20210110_1503'),
    ]

    operations = [
        migrations.CreateModel(
            name='Message',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('from_user', models.IntegerField()),
                ('message', models.TextField()),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('room', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='chat_app_service.room')),
            ],
        ),
        migrations.CreateModel(
            name='ChatPrivate',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('from_user', models.IntegerField()),
                ('message', models.TextField()),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('to_user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='chat_app_service.user')),
            ],
        ),
    ]
